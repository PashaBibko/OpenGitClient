#pragma once

#include <sstream>
#include <string>

#include <git2.h>

template<typename StreamTy, typename Ty>
concept StreamSupportedType = requires (StreamTy& os, Ty instance) {
    { os << instance } -> std::same_as<StreamTy&>;
};

class AppContext {
    std::string m_AppDataFolder;
    bool m_AppDataFolderExists;

    void OutputLogMessage(const std::string& message);

public:
    git_repository* m_SelectedRepo = nullptr;
    std::string m_LastOpenedRepository;

    AppContext() = delete;
    explicit AppContext(const std::string& appDataFolder);

    ~AppContext();

    template<typename... Args>
        requires (StreamSupportedType<std::ostream, Args> && ...)
    void Log(Args&&... args) {
        std::ostringstream oss{};
        (oss << ... << std::forward<Args>(args));
        OutputLogMessage(oss.str());
    }

    template<typename... Args>
        requires (StreamSupportedType<std::ostream, Args> && ...)
    void LogError(Args&&... args) {
        std::ostringstream oss{};
        (oss << ... << std::forward<Args>(args));
        OutputLogMessage(oss.str());
    }
};
