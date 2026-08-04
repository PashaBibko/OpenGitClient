#include "AppContext.h"

#include <filesystem>
#include <fstream>
#include <glaze/glaze.hpp>
#include <iostream>

namespace fs = std::filesystem;

// Used implicitly by glaze
// ReSharper disable once CppUseInternalLinkage
struct PersistentAppContextData {
    std::string LastOpenedRepository;
};

void AppContext::OutputLogMessage(const std::string& message) {
    std::cout << message << std::endl;
}

AppContext::AppContext(const std::string& appDataFolder)
    : m_AppDataFolder(appDataFolder), m_AppDataFolderExists(false) {
    // Starts up lib git as it is required for anything else to work
    git_libgit2_init();

    // Checks that the app data folder exists, if it doesn't it logs a message, we could try to
    // create the folder but that is already handled by C# so it must have failed for a reason.
    if (!(fs::exists(m_AppDataFolder) && fs::is_directory(m_AppDataFolder))) {
        LogError("App data folder [", m_AppDataFolder, "] does not exist.");
        return;
    }

    // Opens the user data file if it exists, will only ever no exist on initial boot of the
    // software or if the user has deleted the appdata folder for the application
    const fs::path userDataPath = fs::path(m_AppDataFolder) / "userdata.json";
    if (!fs::exists(userDataPath)) {
        return;  // Can safely return with the class having default values as there is nothing to
                 // load
    }

    std::ifstream userDataFile(userDataPath);
    if (!userDataFile.is_open()) {
        LogError("Failed to open file");
        return;
    }

    // Reads the serialized user data out of the file
    PersistentAppContextData persistent{};
    std::stringstream buffer{};
    buffer << userDataFile.rdbuf();
    if (const glz::error_ctx ec = glz::read_json(persistent, buffer.str())) {
        LogError("Failed to read json due to [", glz::format_error(ec), ']');
        return;
    }

    // Passes out of the serialized struct so it can be publicly accessed
    m_LastOpenedRepository = persistent.LastOpenedRepository;
}

AppContext::~AppContext() {
    // Frees all remaining lib git objects before shutting it down
    if (m_SelectedRepo != nullptr) {
        git_repository_free(m_SelectedRepo);
        m_SelectedRepo = nullptr;
    }
    git_libgit2_shutdown();

    // Doesn't try to attempt saving if the folder doesn't exist or it can't open the file
    if (!m_AppDataFolderExists) {
        return;
    }

    const fs::path userDataPath = fs::path(m_AppDataFolder) / "userdata.json";
    std::ofstream userDataFile(userDataPath);
    if (!userDataFile.is_open()) {
        LogError("Failed to open userdata file");
        return;
    }

    // Creates an object to hold the persistent data which is then serialized and written to the
    // file
    std::string json{};
    const glz::error_ctx ec = glz::write_json(
        PersistentAppContextData{
            .LastOpenedRepository = m_LastOpenedRepository,
        },
        json);

    if (ec) {
        LogError("Failed to write json due to [", glz::format_error(ec), ']');
        return;
    }

    userDataFile << json;
    userDataFile.close();
}
