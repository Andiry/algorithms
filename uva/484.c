#include<stdio.h>

#define MAX (1<<32 - 1)

int main(void)
{
	int i, index;
	int a[256], b[256], c[256];

	for (i = 0; i < 256; i++)
	{
		a[i] = MAX;
		b[i] = 0;
		c[i] = MAX;
	}

	i = 0;
	while(scanf("%d ", &c[i++]) != EOF);
	printf("\n");

	i = 0;
	while (1) {
		if (c[i] == MAX)
			break;
		index = 0;
		while (1) {
			if (a[index] == MAX) {
				a[index] = c[i];
				b[index] = 1;
				break;
			} else if (a[index] == c[i]) {
				b[index]++;
				break;
			}
			index++;
		}
		i++;
	}

	i = 0;
	while(1) {
		if (a[i] == MAX)
			break;
		printf("%d %d\n", a[i], b[i]);
		i++;
	}

	return 0;
}


