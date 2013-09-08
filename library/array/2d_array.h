
int allocate_2d_array(int ***a, int lena, int lenb)
{
	int i;

	// FIXME: memalloc failure handler required
	*a = (int **)malloc(lena * sizeof(int *));
//	printf("allocate a at 0x%x\n", *a);
	for (i = 0; i < lena; i++) {
		(*a)[i] = (int *)malloc(lenb * sizeof(int));
//		printf("allocate a%d at 0x%x\n", i, (*a)[i]);
	}

	return 0;
}
		
void free_2d_array(int **a, int lena, int lenb)
{
	int i;

	for (i = 0; i < lena; i++)
		free(a[i]);

	free(a);
}
