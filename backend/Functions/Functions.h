#pragma once

#include <string>

#include "../Router/WebFunction.h"

namespace RepoLocation {
    class Choose : public WebFunction<int, void, std::string> {
    public:
        std::string Invoke(int& ctx) override;
    };
} // namespace RepoLocation
