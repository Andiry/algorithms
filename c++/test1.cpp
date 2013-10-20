#include<iostream>

using namespace std;

int fun(int &x)
{
	return x;
}

void swap1(char *&str1, char *&str2)
{
	char *temp = str1;
	str1 = str2;
	str2 = temp;
}

int main(void)
{
	int x =10;
//	cout << fun(x) << endl;
	int *ptr = NULL;
	int &ref  = *ptr;

	char *str1 = "111";
	char *str2 = "222";
	swap1(str1, str2);
	cout << str1 << str2 << endl;


	return 0;
}
