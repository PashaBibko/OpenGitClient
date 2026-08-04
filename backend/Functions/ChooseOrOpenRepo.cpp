#include "../Functions.h"

#include <string>

#include <nfd.h>

static std::optional<std::string> GetUserChosenPath(AppContext& ctx) {
    // Initializes NFD so it can open a native file explorer window
    if (NFD_Init() != NFD_OKAY) {
        ctx.LogError("NFD_Init() failed [", NFD_GetError(), ']');
        return std::nullopt;
    }

    // Opens the window and gets the user to select a folder
    nfdchar_t* outPath = nullptr;
    std::string chosenPath;

    if (const nfdresult_t rc = NFD_PickFolder(&outPath, nullptr); rc == NFD_OKAY) { // nullptr = no default location
        chosenPath = outPath;
    } else if (rc != NFD_CANCEL) { // Doesn't class user canceling as failing
        ctx.LogError("NFD_PickFolder() failed [", NFD_GetError(), ']');
        return std::nullopt;
    }

    // Windows sometimes returns empty paths, instead of returning NFD_CANCEL so we have to do
    // an additional check, ALSO FUCK WINDOWS AND MICROSOFT WITH THEIR TERRIBLE CODED SOFTWARE
    if (chosenPath.empty()) {
        return std::nullopt;
    }

    // Cleans up all allocated resources before returning
    if (outPath != nullptr) {
        NFD_FreePath(outPath);
        outPath = nullptr;
    }
    NFD_Quit();
    return chosenPath;
}

static Repo::ChooseResult TryDiscoverRepositoryAt(const std::string& path, AppContext& ctx) {
    // Frees/Destroys the old repository (if there was one)
    if (ctx.m_SelectedRepo != nullptr) {
        git_repository_free(ctx.m_SelectedRepo);
        ctx.m_SelectedRepo = nullptr;
    }

    // Discovers the repository by looking back directories until it finds one with a .git folder
    git_buf repoPath = {.ptr = nullptr};
    if (git_repository_discover(&repoPath, path.c_str(), 0, nullptr) < 0) {
        const git_error* err = git_error_last();
        ctx.LogError("No repo found: ", err ? err->message : "unknown error");

        git_buf_dispose(&repoPath);
        return {.Filepath = path, .IsRepository = false};
    }

    // Opens the repository with the discovered path
    git_repository_open(&ctx.m_SelectedRepo, repoPath.ptr);
    std::string repoLocation = repoPath.ptr;
    git_buf_dispose(&repoPath);

    static constexpr std::string SUFFIX = ".git/";
    if (repoLocation.ends_with(SUFFIX)) {
        repoLocation.erase(repoLocation.size() - SUFFIX.size());
    }

    // Stores the location in the app context before returning
    ctx.m_LastOpenedRepository = repoLocation;
    return {.Filepath = repoLocation, .IsRepository = true};
}

Repo::ChooseResult Repo::Choose::Invoke(AppContext& ctx) {
    // Gets the folder path chosen by the user
    const std::optional chosenPath = GetUserChosenPath(ctx);
    if (chosenPath == std::nullopt) {
        ctx.Log("User chose not to select a folder");
        return {.Filepath = "", .IsRepository = false};
    }

    // Tries to open the git repository at the chosen location
    return TryDiscoverRepositoryAt(chosenPath.value(), ctx);
}

bool Repo::Open::Invoke(AppContext& ctx, const std::string& path) {
    git_repository* repo = nullptr;
    if (const int ec = git_repository_open(&repo, path.c_str()); ec == 0) {
        ctx.m_LastOpenedRepository = path;
        ctx.m_SelectedRepo = repo;

        return true;
    }

    return false;
}
