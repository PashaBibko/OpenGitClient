#pragma once

#include <git2.h>

struct AppContext {
    git_repository* m_SelectedRepo = nullptr;

    ~AppContext() {
        if (m_SelectedRepo != nullptr) {
            git_repository_free(m_SelectedRepo);
            m_SelectedRepo = nullptr;
        }
    }
};
