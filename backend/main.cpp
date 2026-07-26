#include "InteropBindings/OpenGitClientInterop.h"

#include "Router/WebFunctionRouter.h"
#include "Router/WebFunction.h"

#include <iostream>

WebFunctionRouter router;

struct TestObject {
    int Value;
};

template<> struct glz::meta<TestObject> {
    using T = TestObject;

    static constexpr auto value = object
    (
        "value", &T::Value
    );
};

class BasicWebFunction : public WebFunction<TestObject> {
public:
    void Invoke(const TestObject& object) override {
        std::cout << "Hello there: " << object.Value << std::endl;
    }
};

class ReturningWebFunction : public WebFunction<void, TestObject> {
public:
    TestObject Invoke() override {
        std::cout << "Called function with return value" << std::endl;
        return TestObject{.Value = 420};
    }
};

class VoidWebFunction : public WebFunction<void> {
public:
    void Invoke() override {
        std::cout << "Void function called." << std::endl;
    }
};

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
    router.AddFunction<BasicWebFunction>("BasicFunc");
    router.AddFunction<VoidWebFunction>("VoidFunc");
    router.AddFunction<ReturningWebFunction>("ReturnFunc");

    OpenGitClientInterop::BindMessageReceiverCallback(BasicCallback);
    OpenGitClientInterop::StartPhotino();
}
