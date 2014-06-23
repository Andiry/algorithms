#include<stdio.h>
#include<string.h>
#include<malloc.h>



int main(void)
{
	char *str, *c;
	int i, j, count, tmp, tmp1;
	int a[10][26];
	int b[10][26];
	int result[10];

	for (i = 0; i < 10; i++) {
		for (j = 0; j < 26; j++) {
			a[i][j] = 0;
			b[i][j] = 0;
		}
	}

	for (i = 0; i < 10; i++)
		result[i] = 0;

	count = 0;
	str = malloc(100);
	while(1) {
		memset(str, 100, 0);
		scanf("%s", str);
		if (strcmp(str, "#") == 0)
			break;
		c = str;
		while(*c != '\0') {
			a[count][*c - 'a']++;
			c++;
		}
		count++;
	}

	i = 0;
	while(1) {
		memset(str, 100, 0);
		scanf("%s", str);
		if (strcmp(str, "#") == 0)
			break;
		c = str;
		printf("%s\n", str);
		while(*c != '\0') {
			if (*c != ' ')
				b[i][*c - 'a']++;
			c++;
		}
		i++;
	}

	printf("%d %d\n", count, i);
	for (j = 0; j < i; j++) {
		for (tmp = 0; tmp < count; tmp++) {
			for (tmp1 = 0; tmp1 < 26; tmp1++) {
				if (a[tmp][tmp1] > b[j][tmp1])
					break;
			}
			if (tmp1 == 26)
				result[j]++;
		}
	}

	printf("%d %d\n", count, i);
	for (j = 0; j < i; j++)	
		printf("%d\n", result[j]);
	free(str);

	return 0;
}
