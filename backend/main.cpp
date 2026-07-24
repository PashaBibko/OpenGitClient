#include "InteropBindings/OpenGitClientInterop.h"

#include <iostream>

static void BasicCallback() {
    std::cout << "Ran BasicCallback" << std::endl;
}

int main() {
    OpenGitClientInterop::BindCallback("BasicCallback", BasicCallback);

    OpenGitClientInterop::StartPhotino();
}
