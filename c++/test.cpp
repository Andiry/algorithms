#include<iostream>
using namespace std;


class A
{
private:
	int value;

public:
	A(int n) {value = n;}
	A() {cout << "Construct A" << endl;}
	A(const A &other) {value = other.value;}
	A& operator=(const A &other);

	void print() {cout << "non-const." << endl;}
	void print() const {cout << "const" << endl;}
};

void print1(char *a) {cout << "non-const. " << a << endl;}
void print1(const char *a) {cout << "const. " << a << endl;}

void fun(int &i) {cout << "non-const fun " << i << endl;}
void fun(const int &i) {cout << "const fun " << i << endl;}

A& A::operator=(const A &other)
{
	if (this == &other)
		return *this;

	this->value = other.value;
	return *this;
}

class B
{
private:
	static A a;
public:
	B() {cout << "construct B"<< endl;}
};

int main(void)
{
	A a = 10;
	A b = a;
	const A c(20);
	b.print();
	c.print();
	char *p1 = "p1";
	const char *p2 = "p2";

	print1(p1);
	print1(p2);

	int i = 10;
	const int i2 = 20;
	fun(i);
	fun(i2);

	A d;
	B e;


	return 0;
}
	
