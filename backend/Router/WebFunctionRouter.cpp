#include "WebFunctionRouter.h"

WebFunctionRouter* WebFunctionRouter::sInstance = nullptr;

char* WebFunctionRouter::Route(const char* funcName, const char* serialized) {
    return sInstance->RouteInner(funcName, serialized);
}

char* WebFunctionRouter::RouteInner(const char* funcName, const char* serialized) {
    m_AppContext.Log("Web function [", funcName, "] called.");

    // Deletes anything remaining in the shared buffer
    delete m_SharedBuffer;
    m_SharedBuffer = nullptr;

    // Finds and invokes the function (if it can)
    std::optional<std::string> webFunctionResult = std::nullopt;
    if (const auto iter = m_Functions.find(funcName); iter != m_Functions.end()) {
        webFunctionResult = iter->second->InvokeInner(m_AppContext, serialized);
    } else {
        m_AppContext.LogError("Failed to find [", funcName, "] web function.");
    }

    // Returns early if the function had no result or could not be found
    if (webFunctionResult == std::nullopt) {
        return nullptr;
    }

    // Copies the result into the shared buffer
    const std::string& serializedResult = webFunctionResult.value();
    const size_t serializedLength = serializedResult.size();
    m_SharedBuffer = new char[serializedLength];

    std::ranges::copy(serializedResult, m_SharedBuffer);
    m_SharedBuffer[serializedLength] = '\0';
    return m_SharedBuffer;
}

std::optional<std::string> WebFunctionRouter::InvokeFunction(const std::string& funcName, const char* serialized) {
    if (const auto iter = m_Functions.find(funcName); iter != m_Functions.end()) {
        return iter->second->InvokeInner(m_AppContext, serialized);
    }

    m_AppContext.LogError("Failed to find [", funcName, "] web function.");
    return std::nullopt;
}
