
int partition1(int *array, int start, int end)
{
	int sentry = array[start];

	while(start < end) {
		while(sentry <= array[end] && start < end)
			end--;

		array[start] = array[end];
		while(sentry >= array[start] && start < end)
			start++;

		array[end] = array[start];
//		print_array(array, length);
	}

	array[start] = sentry;

//	print_array(array, length);
	return start;
}

int partition(int *array, int start, int end)
{
	int sentry = array[end];
	int pos = start;

	while(pos < end) {
		if (array[pos] < sentry) {
			swap(array, start, pos);
			start++;
		}
		pos++;
	}

	swap(array, start, end);

	return start;
}

void quick_sort(int * array, int start, int end)
{
	int mid;

	if (start >= end)
		return;

	mid = partition(array, start, end);

	quick_sort(array, start, mid - 1);
	quick_sort(array, mid + 1, end);
}
