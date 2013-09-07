
int * format_random_array(int length, int seed)
{
	int *array, i;

	if (length <= 0)
		return NULL;

	array = malloc(length * sizeof(int));
	if (!array)
		return NULL;

	srand(seed);

	for (i = 0; i < length; i++)
		array[i] = rand();

	return array;
}

void print_array(int *array, int length)
{
	int i;

	for (i = 0; i < length; i++)
		printf("%d   ", array[i]);

	printf("\n");
}
