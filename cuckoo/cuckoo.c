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

static inline unsigned long djb_hashl(const char *str, int length)
{
	unsigned long hash = 0;
	int i;

	for (i = 0; i < length; i++) {
		hash = ((hash << 5) + hash) ^ (*str++);
	}

	return hash;
}

static inline unsigned long murmur_hash_64(const char *str, int len,
	unsigned long seed)
{
	const unsigned long m = 0xc6a4a7935bd1e995ULL;
	const int r = 47;

	unsigned long h = seed ^ (len * m);

	const unsigned long * data = (const unsigned long *)str;
	const unsigned long * end = data + (len / 8);

        while (data != end) {
		unsigned long k = *data++;
		k *= m;
		k ^= k >> r;
		k *= m;

		h ^= k;
		h *= m;
	}

	const unsigned char * data2 = (const unsigned char*)data;
	switch (len & 7) {
		case 7: h ^= (unsigned long)(data2[6]) << 48;
		case 6: h ^= (unsigned long)(data2[5]) << 40;
		case 5: h ^= (unsigned long)(data2[4]) << 32;
		case 4: h ^= (unsigned long)(data2[3]) << 24;
		case 3: h ^= (unsigned long)(data2[2]) << 16;
		case 2: h ^= (unsigned long)(data2[1]) << 8;
		case 1: h ^= (unsigned long)(data2[0]);
		h *= m;
	};

	h ^= h >> r;
	h *= m;
	h ^= h >> r;

	return h;
}

static inline unsigned long hash_func1(const char *str, int length)
{
	return djb_hashl(str, length);
//	return BKDRHash(str, length, 31);
//	return murmur_hash_64(str, length, 131);
}

static inline unsigned long hash_func2(const char *str, int length)
{
	return murmur_hash_64(str, length, 31);
//	return BKDRHash(str, length, 131);
//	return djb_hashl(str, length);
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
	int cur_height = cuckoo->height;
	struct cuckoo_internal *internal;
	struct cuckoo_leaf *leaf;
	struct cuckoo_slot *slot = NULL;
	unsigned int right_bits;
	int index = 0;

	/* We do not allow tree height increasing in this method. */
	if (!root)
		return NULL;

	while (cur_height) {
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
		if (cur_height == 1) {
			leaf = (struct cuckoo_leaf *)root;
			index = hashcode & (SLOTS_PER_LEAF - 1);
			slot = &(leaf->slots[index]);
			break;
		}

		/* Internal node */
		internal = (struct cuckoo_internal *)root;
		right_bits = LEAF_BITS + (cur_height - 2) * INTERNAL_BITS;
		index = (hashcode >> right_bits) & (SLOTS_PER_INTERNAL - 1);
		parent = root;
		root = (void *)internal->pointers[index];
		cur_height--;
	}

	return slot;
}

static unsigned long find_value(struct cuckoo *cuckoo,
	struct cuckoo_slot *slot, unsigned int target_tag)
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

static unsigned long clear_tag(struct cuckoo *cuckoo,
	struct cuckoo_slot *slot, unsigned int target_tag)
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
			cuckoo->count--;
			break;
		}
	}

	return 0;
}

static unsigned long cuckoo_lookup_delete(struct cuckoo *cuckoo,
	unsigned long (*p)(struct cuckoo *, struct cuckoo_slot *, unsigned int),
	const char *key, int length)
{
	unsigned long hashcode, hashcode1, hashcode2;
	unsigned int tag;
	void* root = cuckoo->root;
	int height = cuckoo->height;
	struct cuckoo_slot *slot = NULL;
	unsigned long ret;

	if (root == NULL || height == 0)
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
		ret = p(cuckoo, slot, tag);
		if (ret)
			return ret;
	}

	/* Check alternate slot */
	hashcode = hashcode1 ^ tag;
	slot = find_slot(cuckoo, hashcode, 0);
	if (slot) {
		ret = p(cuckoo, slot, tag);
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

	printf("%s: count %lu, size %lu, height %d\n",
			__func__, cuckoo->count, cuckoo->size, cuckoo->height);

	if (cuckoo->height == 0) {
		cuckoo->root = malloc(sizeof(struct cuckoo_leaf));
		if (!cuckoo->root)
			return -ENOMEM;

		memset(cuckoo->root, 0, sizeof(struct cuckoo_leaf));
		cuckoo->size = 512;
		cuckoo->height = 1;
	} else {
		new_root = malloc(sizeof(struct cuckoo_internal));
		if (!new_root)
			return -ENOMEM;

		memset(new_root, 0, sizeof(struct cuckoo_internal));
		new_root->pointers[0] = (u64)cuckoo->root;
		cuckoo->root = new_root;
		cuckoo->size *= 512;
		cuckoo->height++;
	}

	/* FIXME: rehash */
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
static inline int slot_insert(struct cuckoo *cuckoo,
	struct cuckoo_slot *slot, unsigned int tag, unsigned long value)
{
	unsigned long insert_obj = format_obj(tag, value);
	unsigned long obj;
	int i;

	for (i = 0; i < 4; i++) {
		obj = slot->objs[i];
		if (GET_TAG(obj) == 0) {
			slot->objs[i] = insert_obj;
			cuckoo->count++;
			return 0;
		}
	}

	return -1;
}

static int cuckoo_kickout(struct cuckoo *cuckoo, struct cuckoo_slot *slot,
	unsigned long hashcode, unsigned int tag, unsigned long value)
{
	unsigned long victim_obj;
	unsigned int victim_tag;
	unsigned long insert_obj;
	int count = 0;

	while (count < KICKOUT_LIMIT) {
		if (count_free(slot) > 0) {
			slot_insert(cuckoo, slot, tag, value);
			break;
		}

		/* Select victim */
		victim_obj = slot->objs[0];
		victim_tag = GET_TAG(victim_obj);

		/* Kickout */
		insert_obj = format_obj(tag, value);
		slot->objs[0] = insert_obj;

		/* Find new slot */
		tag = victim_tag;
		value = GET_VALUE(victim_obj);
		hashcode = hashcode ^ tag;
		slot = find_slot(cuckoo, hashcode, 1);

		count++;
	}

//	printf("%s: count %lu, size %lu, kickout %d\n",
//			__func__, cuckoo->count, cuckoo->size, count);
	return count;
}

/* Return 0 on success */
int cuckoo_insert(struct cuckoo *cuckoo, const char *key,
	int length, unsigned long value)
{
	struct cuckoo_slot *slot1 = NULL, *slot2 = NULL;
	unsigned long hashcode, hashcode1, hashcode2;
	unsigned int tag;
	int count;
	unsigned long ret;

	if (cuckoo->count >= cuckoo->size * 9 / 10)
		cuckoo_expand(cuckoo);

	hashcode1 = hash_func1(key, length);
	hashcode2 = hash_func2(key, length);
	tag = hashcode2 & TAG_MASK;

	/* Tag must be non-zero */
	if (tag == 0)
		tag = 1;
retry:
	hashcode = hashcode1;
	slot1 = find_slot(cuckoo, hashcode, 1);
	if (slot1 && count_free(slot1) > 0)
		return slot_insert(cuckoo, slot1, tag, value);

	/* Check alternate slot */
	hashcode = hashcode1 ^ tag;
	slot2 = find_slot(cuckoo, hashcode, 1);
	if (slot2 && count_free(slot2) > 0)
		return slot_insert(cuckoo, slot2, tag, value);

	count = cuckoo_kickout(cuckoo, slot2, hashcode, tag, value);
	if (count >= KICKOUT_LIMIT) {
		cuckoo_expand(cuckoo);
		goto retry;
	}

	return -ENOMEM;
}

struct cuckoo *new_cuckoo(int height) {
	struct cuckoo *cuckoo = malloc(sizeof(struct cuckoo));
	unsigned long size = 1;
	int i;

	if (!cuckoo)
		return NULL;

	cuckoo->count = 0;
	if (height <= 0) {
		cuckoo->root = NULL;
		cuckoo->size = 0;
		cuckoo->height = 0;
		return cuckoo;
	}

	if (height > MAX_HEIGHT)
		height = MAX_HEIGHT;

	cuckoo->root = malloc(sizeof(struct cuckoo_leaf));
	if (!cuckoo->root) {
		free(cuckoo);
		return NULL;
	}

	memset(cuckoo->root, 0, sizeof(struct cuckoo_leaf));

	for (i = 0; i < height; i++)
		size *= 512;

	cuckoo->height = height;
	cuckoo->size = size;

	return cuckoo;
}

static int cuckoo_free_tree(void *root, int height) {
	struct cuckoo_internal *node;
	int i;
	int ret = 1;

	if (!root || height == 0)
		return 0;

	if (height == 1) {
		/* Leaf */
		free(root);
		return ret;
	}

	/* Internal node */
	node = (struct cuckoo_internal *)root;
	for (i = 0; i < 512; i++)
		ret += cuckoo_free_tree((void *)node->pointers[i], height - 1);

	free(root);
	return ret;
}

void free_cuckoo(struct cuckoo *cuckoo) {
	int ret;

	printf("cuckoo count %lu, size %lu, height %d\n",
			cuckoo->count, cuckoo->size, cuckoo->height);
	ret = cuckoo_free_tree(cuckoo->root, cuckoo->height);
	printf("%s: free %d pages\n", __func__, ret);

	free(cuckoo);
}

