#include "Interop.h"

#include <iostream>

int main() {
    std::cout << "Hello World!" << std::endl;
    std::cout << OpenGitClientInterop_TestAdd(5, 6) << std::endl;
}