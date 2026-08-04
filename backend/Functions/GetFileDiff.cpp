#include "WebFunctions.h"

std::vector<std::string> Repo::GetFileDiff::Invoke(AppContext& ctx, const std::string& filepath) {
    git_object* headObject = nullptr;
    git_revparse_single(&headObject, ctx.m_SelectedRepo, "HEAD^{tree}");

    git_tree* headTree = nullptr;
    git_tree_lookup(&headTree, ctx.m_SelectedRepo, git_object_id(headObject));

    git_diff* diff = nullptr;
    git_diff_options opts = GIT_DIFF_OPTIONS_INIT;

    std::string mutFilepath = filepath;
    char* paths[] = { mutFilepath.data() };
    opts.pathspec = git_strarray { .strings = paths, .count = 1 };

    git_diff_tree_to_workdir_with_index(&diff, ctx.m_SelectedRepo, headTree, &opts);
    git_diff_num_deltas(diff);

    git_patch *patch = nullptr;
    git_patch_from_diff(&patch, diff, 0);

    git_buf buf = { .ptr = nullptr };
    git_patch_to_buf(&buf, patch);

    std::stringstream stream(buf.ptr);
    std::vector<std::string> lines;
    std::string currentLine;
    while (std::getline(stream, currentLine)) {
        lines.push_back(currentLine);
    }

    git_buf_dispose(&buf);
    git_patch_free(patch);

    return lines;
}
