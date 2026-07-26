#include "WebFunctionRouter.h"

void WebFunctionRouter::InvokeFunction(const std::string& name, const char* serialized) const {
    if (const auto iter = m_Functions.find(name); iter != m_Functions.end()) {
        iter->second->InvokeInner(serialized);
    } else {
        std::cout << "ERROR: Failed to find [" << name << "] web function." << std::endl;
    }
}
