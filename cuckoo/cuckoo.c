#include "cuckoo.h"

u64 cuckoo_lookup(struct cuckoo *cuckoo, char *path) {
	u64 hashcode1, hashcode2;



}

static int cuckoo_increase_level(struct cuckoo *cuckoo) {
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

	
