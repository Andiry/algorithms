#include<stdio.h>

int maxsum(int *a, int n)
{
	int max = 0;
	int temp = 0;
	int i;

	for (i = 0; i < n; i++) {
		if (temp < 0)
			temp = a[i];
		else
			temp += a[i];

		if (max < temp)
			max = temp;
	}

	return max;
}

int main(void)
{
	int a[10] = {1, -8, 6, 3, -1, 5, 7, -2, 0, 1};

	printf("%d\n", maxsum(a,10));
	return 0;
}
