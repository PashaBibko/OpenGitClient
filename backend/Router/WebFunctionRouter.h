#pragma once

#include "WebFunction.h"

class WebFunctionRouter {
    std::unordered_map<std::string, std::unique_ptr<WebFunctionContainer>> m_Functions{};

public:
    void InvokeFunction(const std::string& name, const char* serialized) const;

    template<typename FunctionTy, typename InputTy = FunctionTy::InputType>
        requires std::derived_from<FunctionTy, WebFunction<InputTy>>
    void AddFunction(const char* name) {
        m_Functions[name] = std::make_unique<WebFunctionContainerImpl<FunctionTy>>();
    }
};
