#include<stdio.h>
#include<stdlib.h>

#include"../library/array.h"
#include"../library/sort.h"

int main(void)
{
	int *a, key, location;

	int length = 10;
	int seed = 791;

	a = format_random_array(length, seed);

	print_array(a, length);
	key = a[0];

	heap_sort(a, length);
	print_array(a, length);

	location = binary_search(a, key);

	printf("%d is at %d\n", key, location);

	return 0;
}
