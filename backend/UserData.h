#pragma once

#include <string>

class UserData final {
    std::string m_AppDataFolder;

public:
    std::string m_LastOpenedRepository;

    explicit UserData(const std::string& appDataFolder);
    ~UserData();
};
