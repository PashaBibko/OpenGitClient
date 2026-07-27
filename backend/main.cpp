#include "InteropBindings/OpenGitClientInterop.h"

#include "Router/WebFunctionRouter.h"
#include "Router/WebFunction.h"

#include "Functions/Functions.h"

#include <iostream>

WebFunctionRouter router;

static char* BasicCallback(const char *name, const char *serializedVal) {
    const std::optional result = router.InvokeFunction(name, serializedVal);
    if (!result.has_value()) {
        return nullptr;
    }

    std::string serialized = result.value();
    char* buffer = new char[serialized.size() + 1];
    std::ranges::copy(serialized, buffer);
    buffer[serialized.size()] = '\0';

    return buffer;
}

int main() {
    router.AddFunction<RepoLocation::Choose>("RepoLocation.Choose");

    OpenGitClientInterop::BindMessageReceiverCallback(BasicCallback);
    OpenGitClientInterop::StartPhotino();
}
