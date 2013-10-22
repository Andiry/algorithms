#include<stdio.h>
#include<stdbool.h>

int column[64];
int row[64];

int count;

static bool check(int new_col, int new_row, int put)
{
	int i;

	for (i = 0; i < put; i++) {
		if ((column[i] - new_col == row[i] - new_row) ||
		    (column[i] - new_col == new_row - row[i]))
			return false;
	}

	return true;
} 

static int count_num(int n, int k, int put, int last_col, int last_row)
{
	int i, j, num;

	if (put == k) {
		count++;
		return 0;
	}

	for (i = last_col; i < n; i++) {
		for (j = 0; j < n; j++) {
			if (i == last_col && j <= last_row)
				continue;
			if (check(i, j, put)) {
				column[put] = i;
				row[put] = j;
				count_num(n, k, put+1, i, j);
			}
			continue;
		}
	}
	return 0;
}	

static int count_check(int n, int k)
{
	if (n <= 0 || n > 8)
		return 0;

	if (k <= 0 || k > n * n)
		return 0;

	printf("check %d, %d, 0\n", n, k);
	count_num(n, k, 0, 0, -1);
	return 0;
}	

int main(void)
{
	int n, k;

	while(1) {
		count = 0;
		printf("Please input n and k:");
		scanf("%d %d", &n, &k);
		if (n == 0 && k == 0)
			break;
		count_check(n, k);
		printf("%d\n", count);
	}
	return 0;
}
