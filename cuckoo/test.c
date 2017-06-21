#include "cuckoo.h"


int main(void) {
	struct cuckoo * cuckoo;
	char *key = "Hello World!";
	unsigned long value = 0;
	int i;
	char str[10];

	cuckoo = new_cuckoo(0);

	cuckoo_insert(cuckoo, key, strlen(key), 123);
	value = cuckoo_lookup(cuckoo, key, strlen(key));

	printf("Key: %s, Value: %lu\n", key, value);

	cuckoo_delete(cuckoo, key, strlen(key));
	value = cuckoo_lookup(cuckoo, key, strlen(key));

	printf("Key: %s, Value: %lu\n", key, value);

	for (i = 1; i <= 4096; i++) {
		sprintf(str, "%d", i);
		cuckoo_insert(cuckoo, str, strlen(str), 123);
	}

	free_cuckoo(cuckoo);

	return 0;
}
