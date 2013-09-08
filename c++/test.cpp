#include<iostream>
using namespace std;


class A
{
private:
	int value;

public:
	A(int n) {value = n;}
	A(const A &other) {value = other.value;}
	A& operator=(const A &other);

	void print() {cout << value << endl;}
};

A& A::operator=(const A &other)
{
	if (this == &other)
		return *this;

	this->value = other.value;
	return *this;
}

int main(void)
{
	A a = 10;
	A b = a;
	b.print();
	return 0;
}
	
