#include "InteropBindings/OpenGitClientInterop.h"

#include "Router/WebFunctionRouter.h"
#include "Router/WebFunction.h"

#include "Functions/Functions.h"

#include <iostream>

int main() {
    static WebFunctionRouter<int> router; // Has to be static to be passed into the callback

    router.AddFunction<RepoLocation::Choose>("RepoLocation.Choose");

    OpenGitClientInterop::BindMessageReceiverCallback(
        [](const char* name, const char* serializedVal) -> char* {
            const std::optional result = router.InvokeFunction(name, serializedVal);
            if (!result.has_value()) {
                return nullptr; // Will be converted to "null" by C#
            }

            // Copies the string to a cstr so it can be passed to C#
            // TODO: Add a way for C#/TypeScript to free to avoid memory leaking
            const std::string& serialized = result.value();
            const size_t serializedLen = serialized.size();
            char* buffer = new char[serializedLen + 1];
            std::ranges::copy(serialized, buffer);
            buffer[serializedLen] = '\0';

            return buffer;
        });
    OpenGitClientInterop::StartPhotino();
}
