#include "cuckoo.h"


// BKDR String Hash Function
// Seed = 31 131 1313 13131 131313 etc..
static inline unsigned long BKDRHash(const char *str, int length,
	unsigned int seed)
{
	unsigned long hash = 0;
	int i;

	for (i = 0; i < length; i++) {
		hash = hash * seed + (*str++);
	}

	return hash;
}

static inline unsigned long hash_func1(const char *str, int length)
{
	return BKDRHash(str, length, 131);
}

static inline unsigned long hash_func2(const char *str, int length)
{
	return BKDRHash(str, length, 31);
}

/*
 * Bits         9         9        7
 * --------|---------|---------|-------|
 * Level        3         2        1
 */
static struct cuckoo_slot * find_slot(struct cuckoo *cuckoo,
	unsigned long hashcode, int allocate)
{
	void *root = cuckoo->root;
	void *parent = NULL;
	int cur_level = cuckoo->level;
	struct cuckoo_internal *internal;
	struct cuckoo_leaf *leaf;
	struct cuckoo_slot *slot = NULL;
	unsigned int right_bits;
	int index = 0;

	/* We do not allow tree height increasing in this method. */
	if (!root)
		return NULL;

	while (cur_level) {
		if (!root) {
			if (allocate == 0)
				return NULL;

			root = malloc(PAGE_SIZE);
			if (!root)
				return NULL;

			memset(root, 0, PAGE_SIZE);
			if (parent) {
				internal = (struct cuckoo_internal *)parent;
				internal->pointers[index] = (unsigned long)root;
			}
		}

		/* Leaf node */
		if (cur_level == 1) {
			leaf = (struct cuckoo_leaf *)root;
			index = hashcode & (SLOTS_PER_LEAF - 1);
			slot = &(leaf->slots[index]);
			break;
		}

		/* Internal node */
		internal = (struct cuckoo_internal *)root;
		right_bits = LEAF_BITS + (cur_level - 2) * INTERNAL_BITS;
		index = (hashcode >> right_bits) & (SLOTS_PER_INTERNAL - 1);
		parent = root;
		root = (void *)internal->pointers[index];
		cur_level--;
	}

	return slot;
}

static unsigned long find_value(struct cuckoo_slot *slot,
	unsigned int target_tag)
{
	unsigned long value;
	unsigned int tag;
	int i;

	if (!slot)
		return 0;

	for (i = 0; i < 4; i++) {
		value = slot->objs[i];
		tag = GET_TAG(value);
		if (tag == target_tag) {
			/* FIXME: Check real value */
			return GET_VALUE(value);
		}
	}

	return 0;
}

static unsigned long clear_tag(struct cuckoo_slot *slot,
	unsigned int target_tag)
{
	unsigned long value;
	unsigned int tag;
	int i;

	if (!slot)
		return 0;

	for (i = 0; i < 4; i++) {
		value = slot->objs[i];
		tag = GET_TAG(value);
		if (tag == target_tag) {
			/* FIXME: Check real value */
			slot->objs[i] = 0;
			break;
		}
	}

	return 0;
}

static unsigned long cuckoo_lookup_delete(struct cuckoo *cuckoo,
	unsigned long (*p)(struct cuckoo_slot *, unsigned int),
	const char *key, int length)
{
	unsigned long hashcode, hashcode1, hashcode2;
	unsigned int tag;
	void* root = cuckoo->root;
	int level = cuckoo->level;
	struct cuckoo_slot *slot = NULL;
	unsigned long ret;

	if (root == NULL || level == 0)
		return 0;

	hashcode1 = hash_func1(key, length);
	hashcode2 = hash_func2(key, length);
	tag = hashcode2 & TAG_MASK;

	/* Tag must be non-zero */
	if (tag == 0)
		tag = 1;

	hashcode = hashcode1;
	slot = find_slot(cuckoo, hashcode, 0);
	if (slot) {
		ret = p(slot, tag);
		if (ret)
			return ret;
	}

	/* Check alternate slot */
	hashcode = hashcode1 ^ tag;
	slot = find_slot(cuckoo, hashcode, 0);
	if (slot) {
		ret = p(slot, tag);
		if (ret)
			return ret;
	}

	return 0;
}

unsigned long cuckoo_lookup(struct cuckoo *cuckoo, const char *key,
	int length)
{
	return cuckoo_lookup_delete(cuckoo, &find_value, key, length);
}

unsigned long cuckoo_delete(struct cuckoo *cuckoo, const char *key,
	int length)
{
	return cuckoo_lookup_delete(cuckoo, &clear_tag, key, length);
}

/*
 * Cuckoo hash table increases as B-tree. Level 1 is leaf.
 */
static int cuckoo_expand(struct cuckoo *cuckoo)
{
	struct cuckoo_internal *new_root;

	if (cuckoo->level == 0) {
		cuckoo->root = malloc(sizeof(struct cuckoo_leaf));
		if (!cuckoo->root)
			return -ENOMEM;

		memset(cuckoo->root, 0, sizeof(struct cuckoo_leaf));
		cuckoo->size = 128;
		cuckoo->level = 1;
	} else {
		new_root = malloc(sizeof(struct cuckoo_internal));
		if (!new_root)
			return -ENOMEM;

		memset(new_root, 0, sizeof(struct cuckoo_internal));
		new_root->pointers[0] = (u64)cuckoo->root;
		cuckoo->root = new_root;
		cuckoo->size *= 512;
		cuckoo->level++;
	}

	return 0;
}

static inline int count_free(struct cuckoo_slot *slot)
{
	unsigned long obj;
	int count = 0;
	int i;

	for (i = 0; i < 4; i++) {
		obj = slot->objs[i];
		if (GET_TAG(obj) == 0)
			count++;
	}

	return count;
}

/* Return 0 on success */
static inline int slot_insert(struct cuckoo_slot *slot, unsigned int tag,
	unsigned long value)
{
	unsigned long insert_value = format_obj(tag, value);
	unsigned long obj;
	int i;

	for (i = 0; i < 4; i++) {
		obj = slot->objs[i];
		if (GET_TAG(obj) == 0) {
			slot->objs[i] = insert_value;
			return 0;
		}
	}

	return -1;
}

/* Return 0 on success */
int cuckoo_insert(struct cuckoo *cuckoo, const char *key,
	int length, unsigned long value)
{
	unsigned long hashcode, hashcode1, hashcode2;
	unsigned int tag;
	void* root = cuckoo->root;
	int level = cuckoo->level;
	struct cuckoo_slot *slot1 = NULL, *slot2 = NULL;
	unsigned long ret;

	if (cuckoo->count >= cuckoo->size * 9 / 10)
		cuckoo_expand(cuckoo);

	hashcode1 = hash_func1(key, length);
	hashcode2 = hash_func2(key, length);
	tag = hashcode2 & TAG_MASK;

	/* Tag must be non-zero */
	if (tag == 0)
		tag = 1;

	hashcode = hashcode1;
	slot1 = find_slot(cuckoo, hashcode, 1);
	if (slot1 && count_free(slot1) > 0)
		return slot_insert(slot1, tag, value);

	/* Check alternate slot */
	hashcode = hashcode1 ^ tag;
	slot2 = find_slot(cuckoo, hashcode, 1);
	if (slot2 && count_free(slot2) > 0)
		return slot_insert(slot2, tag, value);

	/* FIXME: kickout */
	return -ENOMEM;
}

struct cuckoo *new_cuckoo(void) {
	struct cuckoo *ret = malloc(sizeof(struct cuckoo));

	ret->root = NULL;
	ret->size = 0;
	ret->count = 0;
	ret->level = 0;

	return ret;
}

static void cuckoo_free_tree(void *root, int level) {
	struct cuckoo_internal *node;
	int i;

	if (!root || level == 0)
		return;

	if (level == 1) {
		/* Leaf */
		free(root);
		return;
	}

	/* Internal node */
	node = (struct cuckoo_internal *)root;
	for (i = 0; i < 512; i++)
		cuckoo_free_tree((void *)node->pointers[i], level - 1);

	free(root);
}

void free_cuckoo(struct cuckoo *cuckoo) {
	cuckoo_free_tree(cuckoo->root, cuckoo->level);
	
	free(cuckoo);
}

	