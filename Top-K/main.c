#include<stdio.h>
#include<stdlib.h>

#include"../library/common.h"
#include"../library/array/array.h"
#include"../library/sort/heap.h"

#define MAX_INT ((1 << 30) - 1)

int main(int argc, char **argv)
{
	int *a, *heap;

	int length = 12;
	int seed, ret;
	int range = 256;
	int K = 4, i;

	if (argc < 2)
		seed = 124;
	else
		seed = atoi(argv[1]);

	a = format_random_array(length, seed, range);
	if (!a)
		goto out;

	print_array(a, length);

	heap = new int[K];
	for (i = 0; i < K; i++)
		heap[i] = MAX_INT;

	heap_init(heap, K, 0);
	for (i = 0; i < length; i++)
		heap_insert(heap, a[i], K, 0);

	print_array(heap, K);

out:
	free_array(a);
	delete [] heap;
	return ret;
}
