
void heap_adjust(int *array, int pos, int length, int min)
{
	int left, right;
	int candidate = pos;

	left = pos * 2 + 1;
	right = pos * 2 + 2;

	if (min) {
		if (left < length && array[pos] > array[left])
			candidate = left;
		if (right < length && array[candidate] > array[right])
			candidate = right;
	} else {
		if (left < length && array[pos] < array[left])
			candidate = left;
		if (right < length && array[candidate] < array[right])
			candidate = right;
	}
		
	if (pos == candidate)
		return;

	swap(array, pos, candidate);

	heap_adjust(array, candidate, length, min);
}

void heap_init(int *array, int length, int min)
{
	int i;

	for (i = length / 2 - 1; i >= 0; i--) {
		heap_adjust(array, i, length, min);
	}
}

int extract_heap_root(int *array, int length, int min)
{
	int ret = array[0];

	swap(array, 0, length - 1);
	heap_adjust(array, 0, length - 1, min);

	return ret;
}

int heap_sort(int **array, int length, int min)
{
	int *b = static_cast<int*>(malloc(length * sizeof(int)));
	int i;
	int temp_len = length;

	if (!b)
		return -1;

	for (i = 0; i < length; i++) {
		b[i] = extract_heap_root(*array, temp_len, min);
		temp_len--;
	}

	free_array(*array);
	*array = b;

	return 0;
}

void heap_insert(int *heap, int value, int length, int min)
{
	int root = heap[0];

	if ((min && value <= root) || (!min && value >= root))
		return;

	heap[0] = value;
	heap_adjust(heap, 0, length, min);

	return;
}

