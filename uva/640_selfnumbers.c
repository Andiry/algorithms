#include<stdio.h>

static int gen_selfnumber(int n)
{
	int temp = n;

	while(temp) {
		n += temp % 10;
		temp = temp / 10;
	}

	return n;
}

int main(void)
{
	int a[1000000];
	int i;
	int j;

	for (i = 1; i <= 1000000; i++) {
		a[i - 1] = 1;
	}
	for (i = 1; i <= 1000000; i++) {
		if (!a[i - 1])
			continue;
		printf("%d\n", i);
		j = gen_selfnumber(i);
		while (j <= 1000000) {
			a[j - 1] = 0;
			j = gen_selfnumber(j);
			if (a[j - 1] == 0)
				break;
		}
	}

	return 0;
}
