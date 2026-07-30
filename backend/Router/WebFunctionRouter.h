#pragma once

#include "WebFunction.h"

template<typename AppContextTy>
class WebFunctionRouter {
    std::unordered_map<std::string, std::unique_ptr<WebFunctionContainer<AppContextTy>>> m_Functions{};
    AppContextTy m_AppContext;

public:
    template<typename... Args>
    explicit WebFunctionRouter(Args&&... args) : m_AppContext(std::forward<Args>(args)...) {
    }

    std::optional<std::string> InvokeFunction(const std::string& name, const char* serialized) {
        if (const auto iter = m_Functions.find(name); iter != m_Functions.end()) {
            return iter->second->InvokeInner(m_AppContext, serialized);
        }

        std::cout << "ERROR: Failed to find [" << name << "] web function." << std::endl;
        return std::nullopt;
    }

    template<typename FunctionTy, typename InputTy = FunctionTy::InputType, typename OutputTy = FunctionTy::OutputType>
        requires std::derived_from<FunctionTy, WebFunction<AppContextTy, InputTy, OutputTy>>
    void AddFunction(const char* name) {
        m_Functions[name] = std::make_unique<WebFunctionContainerImpl<AppContextTy, FunctionTy>>();
    }
};
