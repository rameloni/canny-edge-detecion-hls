#include <iostream>

void add(int a, int b, int *y);

int main ()
{
	int a = 1;
	int b = 2;
	int y;
	add(a, b, &y);
	std::cout << y << std::endl;
	return 0;
}
