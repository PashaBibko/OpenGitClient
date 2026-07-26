#include "WebFunctionRouter.h"

std::optional<std::string> WebFunctionRouter::InvokeFunction(const std::string& name, const char* serialized) const {
    if (const auto iter = m_Functions.find(name); iter != m_Functions.end()) {
        return iter->second->InvokeInner(serialized);
    }

    std::cout << "ERROR: Failed to find [" << name << "] web function." << std::endl;
    return std::nullopt;
}


