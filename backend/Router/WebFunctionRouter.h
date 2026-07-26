#pragma once

#include "WebFunction.h"

class WebFunctionRouter {
    std::unordered_map<std::string, std::unique_ptr<WebFunctionContainer>> m_Functions{};

public:
    std::optional<std::string> InvokeFunction(const std::string& name, const char* serialized) const;

    template<typename FunctionTy, typename InputTy = FunctionTy::InputType, typename OutputTy = FunctionTy::OutputType>
        requires std::derived_from<FunctionTy, WebFunction<InputTy, OutputTy>>
    void AddFunction(const char* name) {
        m_Functions[name] = std::make_unique<WebFunctionContainerImpl<FunctionTy>>();
    }
};
