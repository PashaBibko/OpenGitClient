#pragma once

#include <string>

#include "Router/WebFunction.h"
#include "AppContext.h"

namespace Repo {
    struct ChooseResult {
        std::string Filepath;
        bool IsRepository;
    };

    class Choose final : public WebFunction<AppContext, void, ChooseResult> {
    public:
        ChooseResult Invoke(AppContext& ctx) override;
    };

    class Open final : public WebFunction<AppContext, std::string, bool> {
    public:
        bool Invoke(AppContext& ctx, const std::string& path) override;
    };

    struct FileStatusBreakdown {
        std::string FileLocation;
        std::string SpecialState;
        std::string StagedState;
        std::string UnstagedState;
    };

    class GetChanges final : public WebFunction<AppContext, void, std::vector<FileStatusBreakdown>> {
    public:
        std::vector<FileStatusBreakdown> Invoke(AppContext& ctx) override;
    };

    class GetFileDiff final : public WebFunction<AppContext, std::string, std::vector<std::string>>{
    public:
        std::vector<std::string> Invoke(AppContext& ctx, const std::string& filepath) override;
    };
} // namespace Repo
