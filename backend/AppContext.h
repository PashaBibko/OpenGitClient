#pragma once

#include <git2.h>

#include "UserData.h"

struct AppContext {
    git_repository* m_SelectedRepo = nullptr;

    UserData m_UserData;

    AppContext() = delete;
    explicit AppContext(const std::string& appDataFolder) : m_UserData(appDataFolder) {
        git_libgit2_init();
    }

    ~AppContext() {
        if (m_SelectedRepo != nullptr) {
            git_repository_free(m_SelectedRepo);
            m_SelectedRepo = nullptr;
        }

        git_libgit2_shutdown();
    }
};
