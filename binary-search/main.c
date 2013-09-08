#include<stdio.h>
#include<stdlib.h>

#include"../array/array.h"
#include"../sort/heap.h"
#include"../search/binary_search.h"

int main(int argc, char **argv)
{
	int *a, key, location;

	int length = 12;
	int seed, ret;
	int range = 256;

	if (argc < 2)
		seed = 124;
	else
		seed = atoi(argv[1]);

	a = format_random_array(length, seed, range);
	if (!a)
		goto out;

	print_array(a, length);
	key = a[0];

	heap_init(a, length, 1);
	print_array(a, length);

	ret = heap_sort(&a, length, 1);
	if (ret)
		goto out;

	print_array(a, length);
	location = binary_search(a, length, key);

	if (location >= 0)
		printf("%d is at %d\n", key, location);

out:
	free_array(a);
	return ret;
}
