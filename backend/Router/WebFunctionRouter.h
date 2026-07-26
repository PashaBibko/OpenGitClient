#pragma once

#include <iostream>

#include "WebFunction.h"

class WebFunctionRouter {
    std::unordered_map<std::string, std::unique_ptr<WebFunctionContainer>> m_Functions{};

public:
    template<typename FunctionTy, typename InputTy>
        requires std::derived_from<FunctionTy, WebFunction<InputTy>>
    void AddFunction(const char* name) {
        m_Functions[name] = std::make_unique<WebFunctionContainerImpl<FunctionTy, InputTy>>();
    }

    void InvokeFunction(const std::string& name, const char* serialized) const {
        if (const auto iter = m_Functions.find(name); iter != m_Functions.end()) {
            iter->second->InvokeInner(serialized);
        } else {
            std::cout << "ERROR: Failed to find [" << name << "] web function." << std::endl;
        }
    }
};