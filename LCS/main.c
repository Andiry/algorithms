/* LCS: longest common serial */

#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>

#include"../library/array/2d_array.h"
//#include"../library/sort/heap.h"
//#include"../library/search/binary_search.h"

#define MAX(a, b) ((a > b) ? (a) : (b))

void print_lcs(int **array, char *a, char *b, int length)
{
	char *c = malloc(length + 1);
	int lena = strlen(a);
	int lenb = strlen(b);
	int i, j;

	c[length] = '\0';

	i = lena;
	j = lenb;

	while(length) {
		if (array[i][j] == array[i - 1][j - 1] + 1 &&
				array[i][j] > array[i - 1][j] &&
				array[i][j] > array[i][j - 1]) {
			c[length - 1] = a[i - 1];
			i--;
			j--;
			length--;
		} else if (array[i][j] == array[i][j - 1]) {
			j--;
		} else {
			i--;
		}
	}

	printf("LCS: %s\n", c);
	free(c);
}

void print_lcs1(int **array, char *a, char *b, int length)
{
	int lena = strlen(a) + 2;
	int lenb = strlen(b) + 2;
	int i, j;

	for (i = 0; i< lena; i++) {
		for (j = 0; j < lenb; j++) {
			if (i == 0 && j <= 1)
				printf("\t");
			else if (i == 0)
				printf("%c\t", b[j - 2]);
			else if (i == 1 && j == 0) 
				printf("\t");
			else if (j == 0)
				printf("%c\t", a[i - 2]);
			else
				printf("%d\t", array[i - 1][j - 1]);
		}
		printf("\n");
	}
				
}

int calculate_lcs(char *a, char *b)
{
	int lena = strlen(a);
	int lenb = strlen(b);
	int **array;
	int ret;
	int i, j;

	ret = allocate_2d_array(&array, lena + 1, lenb + 1);

	for (i = 0; i < lena; i++)
		array[i][0] = 0;
	for (j = 0; j < lenb; j++)
		array[0][j] = 0;

	for (i = 1; i < lena + 1; i++) {
		for (j = 1; j < lenb + 1; j++) {
			if (a[i - 1] == b[j - 1])
				array[i][j] = array[i - 1][j - 1] + 1;
			else
				array[i][j] = MAX(array[i][j - 1], array[i - 1][j]);
		}
	}
 
	ret = array[lena][lenb];

	print_lcs1(array, a, b, ret);
	print_lcs(array, a, b, ret);

	free_2d_array(array, lena + 1, lenb + 1);

	return ret;
}

int main(int argc, char **argv)
{
	char *a = "BDCABA";
	char *b = "ABCBDAB";
	int lcs;

	lcs = calculate_lcs(a, b);

	printf("LCS is %d\n", lcs);

	return 0;
}
