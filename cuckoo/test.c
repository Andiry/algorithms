#include "cuckoo.h"


int main(void) {
	struct cuckoo * cuckoo;

	cuckoo = new_cuckoo();
	free_cuckoo(cuckoo);

	return 0;
}
