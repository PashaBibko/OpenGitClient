#pragma once

#include "../Router/WebFunction.h"
#include "AppContext.h"

namespace AppContextAccess {
    class ExtLog final : public WebFunction<std::string, void> {
    public:
        void Invoke(AppContext &ctx, const std::string& message) override {
            ctx.Log("[Frontend] ", message);
        }
    };

    class ExtLogError final : public WebFunction<std::string, void> {
    public:
        void Invoke(AppContext &ctx, const std::string& message) override {
            ctx.LogError("[Frontend-Error]", message);
        }
    };
} // namespace AppContextAccess
