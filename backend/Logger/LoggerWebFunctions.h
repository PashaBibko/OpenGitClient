#pragma once

#include "../Router/WebFunction.h"
#include "Logger.h"

struct AppContext;

namespace Logger {
    class ExternalLog final : public WebFunction<AppContext, std::string, void> {
    public:
        void Invoke(AppContext &ctx, const std::string& message) override {
            Internal::LogInternal(message);
        }
    };

    class ExternalLogError final : public WebFunction<AppContext, std::string, void> {
    public:
        void Invoke(AppContext &ctx, const std::string& message) override {
            Internal::LogInternal(message);
        }
    };
}
