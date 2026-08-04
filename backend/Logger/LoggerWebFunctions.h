#pragma once

#include "../Router/WebFunction.h"
#include "../AppContext/AppContext.h"

namespace ExternalLoggerInterface {
    class Log final : public WebFunction<AppContext, std::string, void> {
    public:
        void Invoke(AppContext &ctx, const std::string& message) override {
            ctx.Log("[Frontend] ", message);
        }
    };

    class LogError final : public WebFunction<AppContext, std::string, void> {
    public:
        void Invoke(AppContext &ctx, const std::string& message) override {
            ctx.LogError("[Frontend-Error]", message);
        }
    };
} // namespace ExternalLoggerInterface
