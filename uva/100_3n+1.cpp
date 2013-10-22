#include<stdio.h>
#include<iostream>
#include<vector>

using namespace std;

int num_array[10000001];

int process(unsigned int b)
{
	unsigned int count = 0;
	unsigned int end = b;
	unsigned int temp;

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
	unsigned int a, b, ordb, i;
	unsigned int max, cur;
	vector<int> asd1, asd2, asd3;

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

		asd1.push_back(a);
		asd2.push_back(ordb);
		asd3.push_back(max);
	}

	for (i = 0; i < asd1.size(); i++)
		cout << asd1[i] << " " << asd2[i] << " " << asd3[i] << endl;

	return 0;
}
