#include<stdio.h>
#include<stdlib.h>

#include"../library/array/array.h"

bool current_ok(int left, int right)
{
	if (left < 0 || right < 0 || right < left)
		return false;
	return true;
}

void placeparen(int *array, int row, int left, int right)
{
	if (row == 8) {
		print_array(array, 8);
		return;
	}

	array[row] = 0;
	if (current_ok(left - 1, right))
		placeparen(array, row + 1, left - 1, right);

	array[row] = 1;
	if (current_ok(left, right - 1))
		placeparen(array, row + 1, left, right - 1);

	return;
}

int main(void)
{
	int array[8] = {0};

	placeparen(array, 0, 4, 4);
	return 0;
}
