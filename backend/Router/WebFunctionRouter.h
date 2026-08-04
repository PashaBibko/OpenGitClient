#pragma once

#include "WebFunction.h"

class WebFunctionRouter final {
    static WebFunctionRouter* sInstance;

    std::unordered_map<std::string, std::unique_ptr<WebFunctionContainer>> m_Functions{};
    AppContext m_AppContext;

    char* m_SharedBuffer = nullptr;

    std::optional<std::string> InvokeFunction(const std::string& name, const char* serialized);
    char* RouteInner(const char* funcName, const char* serialized);

public:
    template<typename... Args>
    explicit WebFunctionRouter(Args&&... args) : m_AppContext(std::forward<Args>(args)...) {
        if (sInstance == nullptr) {
            sInstance = this;
        } else {
            std::cout << "Multiple instances of web function router cannot coexist in one static context.\n";
        }
    }

    ~WebFunctionRouter() {
        sInstance = nullptr;
    }

    template<typename FunctionTy, typename InputTy = FunctionTy::InputType, typename OutputTy = FunctionTy::OutputType>
        requires std::derived_from<FunctionTy, WebFunction<AppContext, InputTy, OutputTy>>
    void AddFunction(const char* name) {
        m_Functions[name] = std::make_unique<WebFunctionContainerImpl<FunctionTy>>();
    }

    static char* Route(const char* funcName, const char* serialized);
};
