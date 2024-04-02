#include <iostream>
#include "add.h"

using namespace breeze;

int main()
{
    std::cout << "github first try" << std::endl;

    int a = 1;
    int b = 2;

    printf("a = %d, b = %d\n", a, b);    
    add::add_num(a, b);

    std::cout << a << std::endl;

    return 0;
}