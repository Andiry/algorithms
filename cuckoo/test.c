#include "cuckoo.h"


int main(void) {
	struct cuckoo * cuckoo;
	char *key = "Hello World!";
	unsigned long value = 0;

	cuckoo = new_cuckoo();

	cuckoo_insert(cuckoo, key, strlen(key), 123);
	value = cuckoo_lookup(cuckoo, key, strlen(key));

	printf("Key: %s, Value: %lu\n", key, value);

	free_cuckoo(cuckoo);

	return 0;
}
