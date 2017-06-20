#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>


typedef unsigned long u64;

struct cuckoo {
	void *root;
	int level;
	unsigned long count;
	unsigned long size;
};

#define	PAGE_SIZE		4096
#define	SLOTS_PER_INTERNAL	512
#define INTERNAL_BITS		9
struct cuckoo_internal {
	unsigned long pointers[SLOTS_PER_INTERNAL];
};

struct cuckoo_slot {
	unsigned long objs[4];
};

#define TAG_LEN		16
#define TAG_MASK	((1UL << TAG_LEN) - 1)
#define VALUE_MASK	((1UL << (64 - TAG_LEN)) - 1)
#define GET_TAG(p)	(((p) >> (64 - TAG_LEN)) & TAG_MASK)
#define GET_VALUE(p)	((p) & VALUE_MASK)

static inline unsigned long format_obj(unsigned long tag, unsigned long value) {
	return (value & VALUE_MASK) | (tag << (64 - TAG_LEN));
}

#define	SLOTS_PER_LEAF	128
#define LEAF_BITS	7
struct cuckoo_leaf {
	struct cuckoo_slot slots[SLOTS_PER_LEAF];
};



/* Function prototypes */
struct cuckoo *new_cuckoo(void);
void free_cuckoo(struct cuckoo *cuckoo);
