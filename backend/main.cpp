#include "InteropBindings/OpenGitClientInterop.h"

#include <iostream>

static void BasicCallback(const char* name, const char* serializedVal) {

    if (serializedVal == nullptr) {
        std::cout << name << std::endl;
    }

    else {
        std::cout << name << ": " << serializedVal << std::endl;
    }
}

int main() {
    OpenGitClientInterop::BindMessageReceiverCallback(BasicCallback);
    OpenGitClientInterop::StartPhotino();
}
