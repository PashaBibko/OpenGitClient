#include "../Functions.h"

void Repo::GetFileDiff::Invoke(AppContext& ctx, const std::string& filepath) {
    std::cout << "GetFileDiff::Invoke (" << filepath << ")\n";
}
