#include <iostream>
int main()
{
    std::string str_address;
    char a[15];
    std::cout << "str is :" << &str_address 
              << "char array :" << sizeof(a) << std::endl;
}
