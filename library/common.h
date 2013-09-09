template <class T>
void swap(T *array, int a, int b)
{
	T temp = array[a];

	array[a] = array[b];
	array[b] = temp;
}
