
int binary_search(int *array, int length, int key)
{
	int first, middle, last;

	if (!array || length < 1)
		return -1;

	first = 0;
	last = length - 1;

	while(first <= last) {

		middle = first + ((last - first) >> 1);

		if (key == array[middle])
			return middle;

		if (key < array[middle])
			last = middle - 1;
		else
			first = middle + 1;
	}

	return -1;
}

