#include<stdio.h>

int num_array[10000001];

int process(int b)
{
	int count = 0;
	int end = b;
	int temp;

	if (b <= 10000000 && num_array[b])
		return num_array[b];

	if (b == 1)
		return 1;

	if (b & 1) {
		temp = 1 + process(3 * b + 1);
	} else {
		temp = 1 + process(b / 2);
	}

	if (b <= 10000000)
		num_array[b] = temp;

	return temp;
}

int main(void)
{
	int a, b, ordb, i;
	int max, cur;

	while(scanf("%d %d", &a, &b) == 2) {

		max = 0;
		cur = 0;
		ordb = b;
		while (b >= a) {
			if (num_array[b] == 0)
				num_array[b] == process(b);

			max = max > num_array[b] ? max : num_array[b];
			b--;
		}

		printf("%d %d %d\n", a, ordb, max);
	}

	return 0;
}
