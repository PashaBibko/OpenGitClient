#pragma once

#include <string>

#include "../Router/WebFunction.h"
#include "../AppContext.h"

namespace Repo {
    struct ChooseResult {
        std::string SelectedPath;
        bool HasGitRepository;
    };

    class Choose : public WebFunction<AppContext, void, ChooseResult> {
    public:
        ChooseResult Invoke(AppContext& ctx) override;
    };
} // namespace Repo
