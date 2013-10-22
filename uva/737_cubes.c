#include<stdio.h>
#include<math.h>

#define MAX (1 << 30)

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))

void process(int *a, int *b)
{
	int i = 0;
	int temp;
	int (*f)(int, int);

	for (i = 0; i < 6; i++) {
		if (i % 2 == 0)
			b[i] = max(a[i], b[i]);
		else
			b[i] = min(a[i], b[i]);

		if ((i % 2 == 1) && (b[i] <= b[i - 1]))
			b[i] = b[i - 1] = 0;
	}
}

int main(void)
{
	int i, j, count;
	int a[6], b[6], nums, len;
	int results[100];

	nums = 0;

	while(1) {
		scanf("%d", &count);
		if (count <= 0)
			break;

		b[0] = b[2] = b[4] = -MAX;
		b[1] = b[3] = b[5] = MAX;

		for (i = 0; i < count; i++) {
			scanf("%d %d %d %d", &a[0], &a[2], &a[4], &len);
			a[1] = a[0] + len;
			a[3] = a[2] + len;
			a[5] = a[4] + len;
			process(a, b);
		}

		results[nums] = (b[1] - b[0]) * (b[3] - b[2]) * (b[5] - b[4]);

		nums++;
	}

	for (i = 0; i < nums; i++) {
		printf("%d\n", results[i]);
	}

	return 0;
}


