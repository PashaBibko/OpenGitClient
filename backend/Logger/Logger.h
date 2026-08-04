#pragma once

#include <sstream>

template <typename Ty>
concept LogableType = requires(std::ostream& os, Ty arg) {
    { os << arg } -> std::same_as<std::ostream&>;
};

class Logger final {
    void OutputLogMessage(const std::string& message) {
        std::cout << message << std::endl;
    }

public:
    Logger() = delete;
    explicit Logger(const std::string& outputFolder) {
    }

    ~Logger() = default;

    template<typename... Args>
        requires (LogableType<Args> && ...)
    void LogError(Args&&... args) {
        std::stringstream ss;
        (ss << ... << std::forward<Args>(args));
        OutputLogMessage(ss.str());
    }
};
