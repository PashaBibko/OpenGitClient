#pragma once

#include "../Router/WebFunction.h"
#include "../AppContext.h"

namespace ExternalLoggerInterface {
    class Log final : public WebFunction<AppContext, std::string, void> {
    public:
        void Invoke(AppContext &ctx, const std::string& message) override {
            ctx.m_Logger.Log("[Frontend] ", message);
        }
    };

    class LogError final : public WebFunction<AppContext, std::string, void> {
    public:
        void Invoke(AppContext &ctx, const std::string& message) override {
            ctx.m_Logger.LogError("[Frontend-Error]", message);
        }
    };
} // namespace ExternalLoggerInterface
