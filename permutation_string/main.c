/* LCS: longest common serial */

#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>

#include"../library/common.h"
#include"../library/array/2d_array.h"
//#include"../library/sort/heap.h"
//#include"../library/search/binary_search.h"

#define MAX(a, b) ((a > b) ? (a) : (b))

void print_permut(char *string, char *begin)
{
	int lena = strlen(string);
	char *pch;
	int i, j;

	if (*(begin + 1) == '\0') {
		printf("%s\t", string);
		return;
	}

	for (pch = begin; *pch != '\0'; pch++) {
		swap(string, begin - string, pch - string);
		print_permut(string, begin + 1);
		swap(string, begin - string, pch - string);
	}
				
}

int main(int argc, char **argv)
{
	char a[] = "ABC";

	print_permut(a, a);
	printf("\n");

	return 0;
}
