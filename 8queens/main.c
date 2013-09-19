#include<stdio.h>
#include<stdlib.h>

#include"../library/array/array.h"

bool current_ok(int *array, int num)
{
	int i = 0;
	for (i = 0; i < num; i++) {
		if (array[i] == array[num] ||
			array[i] - array[num] == i - num ||
			array[i] - array[num] == num - i)
			return false;
	}
	return true;
}

void placequeen(int *array, int row)
{
	int i;

	if (row == 8) {
		print_array(array, 8);
		return;
	}

	for (i = 0; i < 8; i++) {
		array[row] = i;
		if (current_ok(array, row))
			placequeen(array, row + 1);
	}
	return;
}

int main(void)
{
	int array[8] = {0};

	placequeen(array, 0);
	return 0;
}
