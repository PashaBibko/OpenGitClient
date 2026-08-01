#pragma once

#include <sstream>

namespace Logger {
    template<typename Ty>
    concept Logable = requires(std::ostream& os, Ty arg)
    {
        { os << arg } -> std::same_as<std::ostream&>;
    };

    namespace Internal {
        void LogInternal(const std::string& message);
    } // namespace Internal

    template<typename... Args>
        requires (Logable<Args> && ...)
    void Log(Args&&... args) {
        std::stringstream ss;
        (ss << ... << std::forward<Args>(args));
        Internal::LogInternal(ss.str());
    }

    template<typename... Args>
        requires (Logable<Args> && ...)
    void LogError(Args&&... args) {
        std::stringstream ss;
        (ss << ... << std::forward<Args>(args));
        Internal::LogInternal(ss.str());
    }
} // namespace Logger
