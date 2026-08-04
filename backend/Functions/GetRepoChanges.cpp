#include "WebFunctions.h"

static std::optional<git_status_list*> GetRepoFileStatus(AppContext& ctx) {
    // Checks that there is an open repo
    if (ctx.m_SelectedRepo == nullptr) {
        ctx.LogError("No repository open, could not find any changes.\n");
        return std::nullopt;
    }

    // Fetches ALL the files with changes (unstaged or not)
    git_status_list* statusList = nullptr;
    git_status_options opts = GIT_STATUS_OPTIONS_INIT;
    opts.show = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
    opts.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED | GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX |
                 GIT_STATUS_OPT_RENAMES_INDEX_TO_WORKDIR;

    if (git_status_list_new(&statusList, ctx.m_SelectedRepo, &opts) < 0) {
        const git_error* err = git_error_last();
        ctx.LogError("Failed to create status list: ", err ? err->message : "unknown error");
        return std::nullopt;
    }

    return statusList;
}

static Repo::FileStatusBreakdown StatusEntryToStatusBreakdown(const git_status_entry* entry) {
    // Fetches the filepath out of the status entry
    const char* filepath = entry->head_to_index ? entry->head_to_index->old_file.path
                                                : entry->index_to_workdir->old_file.path;

    // Checks for the special cases, not separate staged and unstaged states
    const unsigned int status = entry->status;
    if (status & GIT_STATUS_CONFLICTED) {
        return {.FileLocation = filepath,
                .SpecialState = "conflicted",
                .StagedState = "none",
                .UnstagedState = "none"};
    }

    if (status & GIT_STATUS_IGNORED) {
        return {.FileLocation = filepath,
                .SpecialState = "ignored",
                .StagedState = "none",
                .UnstagedState = "none"};
    }

    // Staged state (HEAD -> Index)
    std::string stagedState = "none";
    if (status & GIT_STATUS_INDEX_NEW)
        stagedState = "new";
    else if (status & GIT_STATUS_INDEX_MODIFIED)
        stagedState = "modified";
    else if (status & GIT_STATUS_INDEX_DELETED)
        stagedState = "deleted";
    else if (status & GIT_STATUS_INDEX_RENAMED)
        stagedState = "renamed";
    else if (status & GIT_STATUS_INDEX_TYPECHANGE)
        stagedState = "type-change";

    // Unstaged state (Index -> WorkDir)
    std::string unstagedState = "none";
    if (status & GIT_STATUS_WT_NEW)
        unstagedState = "new";
    else if (status & GIT_STATUS_WT_MODIFIED)
        unstagedState = "modified";
    else if (status & GIT_STATUS_WT_DELETED)
        unstagedState = "deleted";
    else if (status & GIT_STATUS_WT_RENAMED)
        unstagedState = "renamed";
    else if (status & GIT_STATUS_WT_TYPECHANGE)
        unstagedState = "type-change";
    else if (status & GIT_STATUS_WT_UNREADABLE)
        unstagedState = "unreadable";

    return {.FileLocation = filepath,
            .SpecialState = "none",
            .StagedState = stagedState,
            .UnstagedState = unstagedState};
}

std::vector<Repo::FileStatusBreakdown> Repo::GetChanges::Invoke(AppContext& ctx) {
    // Fetches the git status list, needs to be freed later on
    const std::optional statusListResult = GetRepoFileStatus(ctx);
    if (statusListResult == std::nullopt) {
        return {};  // Default value of empty vector
    }
    git_status_list* statusList = statusListResult.value();

    // Creates a vector of the file states to send to the frontend
    std::vector<FileStatusBreakdown> breakdowns;
    const size_t filesChanged = git_status_list_entrycount(statusList);

    breakdowns.reserve(filesChanged);  // Pre allocates, to reduced performance cost of vector
    for (size_t i = 0; i < filesChanged; i++) {
        breakdowns.emplace_back(StatusEntryToStatusBreakdown(git_status_byindex(statusList, i)));
    }

    git_status_list_free(statusList);  // Makes sure to free the status list before returning
    return breakdowns;
}
