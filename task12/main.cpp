#include "interpreter.hpp"

int main(int argc, const char** argv)
{
    Interpreter interpreter { true };
    interpreter.start();

    return 0;
}
