#include "Random.h"
#include <iostream>

int main(void)
{
    std::cout << "Hello, world!" << std::endl;
    std::cout << Random().Gen() << std::endl;

    return 0;
}