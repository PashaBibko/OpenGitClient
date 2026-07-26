#pragma once

#include <glaze/glaze.hpp>

template <typename Ty>
concept JsonParsable = std::is_same_v<Ty, void> || requires
{
    glz::meta<Ty>::value;
};

template <JsonParsable InputTy>
class WebFunction {
public:
    virtual void Invoke(const InputTy& object) = 0;
    virtual ~WebFunction() {}
};

template <>
class WebFunction<void> {
public:
    virtual void Invoke() = 0;
    virtual ~WebFunction() {}
};

//

class WebFunctionContainer {
public:
    virtual void InvokeInner(const char* json) = 0;
    virtual ~WebFunctionContainer() {}
};

template <typename FunctionTy, typename InputTy>
    requires std::derived_from<FunctionTy, WebFunction<InputTy>>
class WebFunctionContainerImpl final : public WebFunctionContainer {
    FunctionTy m_InnerFunction{};
public:
    void InvokeInner(const char* json) override {
        // Does not require any JSON conversion if it takes in void
        if constexpr (std::is_void_v<InputTy>) {
            m_InnerFunction.Invoke();
        }

        // Else has to conver the string to the object
        else {
            InputTy object{};
            if (glz::error_ctx ec = glz::read_json(object, json)) {
                throw std::runtime_error("Error reading json.");
            }
            m_InnerFunction.Invoke(object);
        }
    }
};
