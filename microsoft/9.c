#include<stdio.h>

static int check(int *a, int len)
{
	int root = a[len - 1];
	int i, front, count = 0;
	int *b;

	printf(" check %x, %d\n", (int)a, len);
	if (len == 1 || len == 0)
		return 1;

	else {
		front = 1;
		for (i = 0; i < len - 1; i++) {
			if (front == 1) {
				if (a[i] < root)
					continue;
				else {
					front = 0;
					count = i;
					continue;
				}
			} else {
				if (a[i] > root)
					continue;
				else {
					return 0;
				}
			}
		}
		printf(" check ok %x, %d\n", (int)a, count);
		b = a + count;	
		return (check(a, count) && check(b, len - count -1));
	}
}

int main(void)
{
	int a[10], b;
	int i, len = 0;
	int ret;

	while (scanf("%d", &a[len++]) != EOF);

	len--;

	ret = check(a, len);
	printf("\n");
	printf("%d\n", ret);
	return 0;
}
