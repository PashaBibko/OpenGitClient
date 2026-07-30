#include "UserData.h"

#include <glaze/glaze.hpp>
#include <filesystem>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;

struct SerializedUserData {
    std::string LastOpenedRepository;
};

UserData::UserData(const std::string& appDataFolder) : m_AppDataFolder(appDataFolder) {
    // Checks that the app data folder exists, if it doesn't it logs a message, we could try to
    // create the folder but that is already handled by C# so it must have failed for a reason.
    if (!(fs::exists(m_AppDataFolder) && fs::is_directory(m_AppDataFolder))) {
        std::cout << "WARNING: App data folder [" << m_AppDataFolder << "] does not exist.\n";
        return;
    }

    // Opens the user data file if it exists, will only ever no exist on initial boot of the
    // software or if the user has deleted the appdata folder for the application
    const fs::path userDataPath = fs::path(m_AppDataFolder) / "userdata.json";
    if (!fs::exists(userDataPath)) {
        return; // Can safely return with the class having default values as there is nothing to load
    }

    std::ifstream userDataFile(userDataPath);
    if (!userDataFile.is_open()) {
        std::cout << "ERROR: Failed to open file\n";
        return;
    }

    // Reads the serialized user data out of the file
    SerializedUserData serialized {};
    std::stringstream buffer{};
    buffer << userDataFile.rdbuf();
    if (const glz::error_ctx ec = glz::read_json(serialized, buffer.str())) {
        std::cout << "ERROR: Failed to read json due to [" << glz::format_error(ec) << "]\n";
        return;
    }

    // Passes out of the serialized struct so it can be publicly accessed
    m_LastOpenedRepository = serialized.LastOpenedRepository;
}

UserData::~UserData() {
    // Checks again that the app data directory exists, does not log a warning as in the other check
    // as that one will always be run first, and we do not need two identical warnings of the same
    if (!(fs::exists(m_AppDataFolder) && fs::is_directory(m_AppDataFolder))) {
        return;
    }

    // Opens the user data file and writes the serialized data to it so it can be used on next startup
    const fs::path userDataPath = fs::path(m_AppDataFolder) / "userdata.json";
    std::ofstream userDataFile(userDataPath);
    if (!userDataFile.is_open()) {
        std::cout << "ERROR: Failed to open file to write user data.\n";
        return;
    }

    SerializedUserData serialized {
        .LastOpenedRepository = m_LastOpenedRepository,
    };

    std::string json{};
    if (const glz::error_ctx ec = glz::write_json(serialized, json)) {
        std::cout << "ERROR: Failed to write json due to [" << glz::format_error(ec) << "]\n";
        return;
    }

    userDataFile << json;
    userDataFile.close();
}