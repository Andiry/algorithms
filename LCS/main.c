/* LCS: longest common serial */

#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>

#include"../library/array/2d_array.h"
//#include"../library/sort/heap.h"
//#include"../library/search/binary_search.h"

#define MAX(a, b) ((a > b) ? (a) : (b))

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

	free_2d_array(array, lena + 1, lenb + 1);

	return ret;
}

int main(int argc, char **argv)
{
	char *a = "BDECABA";
	char *b = "ABDECBDAB";
	int lcs;

	lcs = calculate_lcs(a, b);

	printf("LCS is %d\n", lcs);

	return 0;
}
