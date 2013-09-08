#include<stdio.h>
#include<stdlib.h>

#include"../library/common.h"
#include"../library/array/array.h"
#include"../library/sort/qsort.h"

int main(int argc, char **argv)
{
	int *a, key, location;

	int length = 12;
	int seed, ret = 0;
	int range = 256;

	if (argc < 2)
		seed = 124;
	else
		seed = atoi(argv[1]);

	a = format_random_array(length, seed, range);
	if (!a)
		goto out;

	print_array(a, length);

	quick_sort(a, 0, length - 1);

	print_array(a, length);

out:
	free_array(a);
	return ret;
}
