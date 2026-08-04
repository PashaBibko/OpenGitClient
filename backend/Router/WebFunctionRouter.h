#pragma once

#include "WebFunction.h"

class WebFunctionRouter final {
    static WebFunctionRouter* sInstance;

    std::unordered_map<std::string, std::unique_ptr<WebFunctionContainer>> m_Functions{};
    char* m_SharedBuffer = nullptr;

    std::optional<std::string> InvokeFunction(const std::string& name, const char* serialized);
    char* RouteInner(const char* funcName, const char* serialized);

  public:
    AppContext m_AppContext;

    template <typename... Args>
    explicit WebFunctionRouter(Args&&... args) : m_AppContext(std::forward<Args>(args)...) {
        if (sInstance == nullptr) {
            sInstance = this;
        } else {
            m_AppContext.Log(
                "Multiple instances of web function router cannot coexist in one static context.");
        }
    }

    ~WebFunctionRouter() {
        sInstance = nullptr;
    }

    template <typename FunctionTy, typename InputTy = FunctionTy::InputType,
              typename OutputTy = FunctionTy::OutputType, typename... Args>
        requires std::derived_from<FunctionTy, WebFunction<InputTy, OutputTy>>
    void AddFunction(const char* name, Args&&... args) {
        m_AppContext.Log("Added function \"", name, "\"");
        m_Functions[name] =
            std::make_unique<WebFunctionContainerImpl<FunctionTy>>(std::forward<Args>(args)...);
    }

    static char* Route(const char* funcName, const char* serialized);
};
