#pragma once

#include <glaze/glaze.hpp>

#include <iostream>

template <typename Ty>
concept JsonParsable = std::is_same_v<Ty, void> || requires
{
    glz::meta<Ty>::value;
};

template <JsonParsable InputTy>
class WebFunction {
public:
    using InputType = InputTy;

    virtual void Invoke(const InputTy& object) = 0;
    virtual ~WebFunction() = default;
};

template <>
class WebFunction<void> {
public:
    using InputType = void;

    virtual void Invoke() = 0;
    virtual ~WebFunction() = default;
};

class WebFunctionContainer {
public:
    virtual void InvokeInner(const char* json) = 0;
    virtual ~WebFunctionContainer() = default;
};

template <typename FunctionTy, typename InputTy = FunctionTy::InputType>
    requires std::derived_from<FunctionTy, WebFunction<InputTy>>
class WebFunctionContainerImpl final : public WebFunctionContainer {
    FunctionTy m_InnerFunction{};
public:
    void InvokeInner(const char* json) override {
        // Does not require any JSON conversion if it takes in void
        if constexpr (std::is_void_v<InputTy>) {
            // Sends a warning if an object was provided
            if (json != nullptr) {
                std::cout << "WARNING: Input parameter was provided to a void web function." << std::endl;
            }

            m_InnerFunction.Invoke();
        } else {
            // Checks there is an input object provided
            if (json == nullptr) {
                std::cout << "ERROR: No input parameter was provided to a web function." << std::endl;
                return;
            }

            // Constructs the object from the JSON
            InputTy object{};
            if (const glz::error_ctx ec = glz::read_json(object, json)) {
                std::cout << "ERROR: " << "Failed to parse json due to [" << glz::format_error(ec) << "]" << std::endl;
                return;
            }
            m_InnerFunction.Invoke(object);
        }
    }
};
