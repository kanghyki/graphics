#include "Random.h"
#include <iostream>
#include <spdlog/spdlog.h>

int main(void)
{
    std::cout << "Hello, world!" << std::endl;
    std::cout << Random().Gen() << std::endl;
    SPDLOG_INFO("Hello, world!");

    return 0;
}