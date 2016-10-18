#include<stdio.h>
#include<stdlib.h>

#include"../library/array/array.h"

int count = 0;

bool current_ok(int **array, int row, int col, int num)
{
	int i = 0;
	int s_x, s_y;
	int x, y;

	for (i = 0; i < row; i++) {
		if (array[i][col] == num)
			return false;
	}

	for (i = 0; i < col; i++) {
		if (array[row][i] == num)
			return false;
	}

	s_x = row / 3;
	s_y = col / 3;

	for (x = s_x * 3; x < (s_x + 1) * 3; x++) {
		for (y = s_y * 3; y < (s_y + 1) * 3; y++) {
			if (x == row && y == col)
				goto out;
			if (array[x][y] == num)
				return false;
		}
	}

out:
	return true;
}

void sudoku(int **array, int row, int col)
{
	int i;

	if (row == 9) {
		printf("Sudoku %d:\n", count);
		for (i = 0; i < 9; i++)
			print_array(array[i], 9);
		printf("\n");
		count++;
		return;
	}

	for (i = 1; i <= 9; i++) {
		array[row][col] = i;
		if (current_ok(array, row, col, i)) {
			if (col < 8) 
				sudoku(array, row, col + 1);
			else
				sudoku(array, row + 1, 0);
		}
	}
	return;
}

int main(void)
{
	int **array;
	int i;

	array = (int **)malloc(sizeof(int *) * 9);
	for (i = 0; i < 9; i++)
		array[i] = (int *)malloc(sizeof(int) * 9);

	sudoku(array, 0, 0);
	return 0;
}
