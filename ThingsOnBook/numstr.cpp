//numstr.cpp
// following number input with line input
// My question: This book is C++98and 11,
// does it still exist in compiler nowadays?
// answer is  yes 
#include <iostream>
int main()
{
	using namespace std;
	cout << "what year was your house built?\n";
	int year;
	cin >> year;
	cout << "What is its street address?\n";
	char address[80];
	cin.getline(address,80);
	cout << "year built:" << year << endl;
	cout << "address:" << address << endl;
	cout << "Done!\n";
	return 0;
}
