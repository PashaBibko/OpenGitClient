#pragma once

#include <string>

#include "../Router/WebFunction.h"

namespace RepoLocation {
    class Choose : public WebFunction<void, std::string> {
    public:
        std::string Invoke() override;
    };
} // namespace RepoLocation
