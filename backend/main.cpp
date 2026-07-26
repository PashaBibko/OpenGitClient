#include "InteropBindings/OpenGitClientInterop.h"

#include "Router/WebFunctionRouter.h"
#include "Router/WebFunction.h"

#include <iostream>

WebFunctionRouter router;

struct InputObject {
    int Value;
};

template<> struct glz::meta<InputObject> {
    using T = InputObject;

    static constexpr auto value = object
    (
        "value", &T::Value
    );
};

class BasicWebFunction : public WebFunction<InputObject> {
public:
    void Invoke(const InputObject& object) override {
        std::cout << "Hello there: " << object.Value << std::endl;
    }
};

class VoidWebFunction : public WebFunction<void> {
public:
    void Invoke() override {
        std::cout << "Void function called." << std::endl;
    }
};

static char* BasicCallback(const char *name, const char *serializedVal) {
    const std::string result = router.InvokeFunction(name, serializedVal);

    return nullptr;
}

int main() {
    router.AddFunction<BasicWebFunction>("BasicFunc");
    router.AddFunction<VoidWebFunction>("VoidFunc");

    OpenGitClientInterop::BindMessageReceiverCallback(BasicCallback);
    OpenGitClientInterop::StartPhotino();
}
