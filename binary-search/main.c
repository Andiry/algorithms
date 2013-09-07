#include<stdio.h>
#include<stdlib.h>

#include"../library/array.h"
#include"../library/sort.h"

int main(int argc, char **argv)
{
	int *a, key, location;

	int length = 10;
	int seed;
	int range = 256;

	if (argc < 2)
		seed = 124;
	else
		seed = atoi(argv[1]);

	a = format_random_array(length, seed, range);

	print_array(a, length);
	key = a[0];

	heap_sort(a, length, 1);
	print_array(a, length);

//	location = binary_search(a, key);

	printf("%d is at %d\n", key, location);

	free_array(a);

	return 0;
}
