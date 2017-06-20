#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>


typedef unsigned long u64;

struct cuckoo {
	void *root;
	int level;
	unsigned long count;
	unsigned long size;
};

struct cuckoo_internal {
	u64 pointers[512];
};

struct cuckoo_slot {
	u64 objs[4];
};

#define TAG_LEN		16
#define TAG_MASK	((1UL << TAG_LEN) - 1)
#define BLOCK_MASK	((1UL << (64 -TAG_LEN)) - 1)
#define GET_TAG(p)	((p >> (64 - TAG_LEN)) & TAG_MASK)
#define GET_BLOCK(p)	(p & BLOCK_MASK)

static inline u64 format_obj(unsigned long tag, unsigned long block) {
	return block | (tag << 48);
}

struct cuckoo_leaf {
	struct cuckoo_slot slots[128];
};




struct cuckoo *new_cuckoo(void);
void free_cuckoo(struct cuckoo *cuckoo);
