#include "string.hpp"
#include <iostream>

int main(int argc, char** argv)
{
    String string { "01234" };
    std::cout << string << std::endl;

    string.append("56789");
    std::cout << string << std::endl;

    return 0;
}
