#include <iostream>
//know how complier distinguish global-variable
//and local-variables
void localVariable();
int main()
{
	int i,j;
	std::cout << &i << std::endl << &j << std::endl;
	localVariable();
	return 0;
}
void localVariable()
{
	int i,j;
	std::cout << &i<< std::endl <<&j;
	return;
}
