
void swap(int *array, int a, int b)
{
	int temp = array[a];

	array[a] = array[b];
	array[b] = temp;
}

void heap_adjust(int *array, int pos, int length, int min)
{
	int left, right;
	int candidate;

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
}

void heap_sort(int *array, int length, int min)
{
	int i;

	for (i = length / 2 - 1; i >= 0; i--) {
		heap_adjust(array, i, length, min);
	}
}
	
