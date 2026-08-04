#include "OpenGitClientInterop.h"

#include "ExternalFunctions.h"

void OpenGitClientInterop::BindMessageReceiverCallback(OpenGitClientMessageReceiverCallback callback) {
    OpenGitClientInterop_BindMessageReceiver(callback);
}

void OpenGitClientInterop::StartPhotino() {
    OpenGitClientInterop_StartPhotino();
}

std::string OpenGitClientInterop::GetAppDataDir() {
    char* directory = static_cast<char*>(OpenGitClientInterop_GetAppDataDir());
    std::string appdata = directory;
    OpenGitClientInterop_FreePtr(directory);  // Frees up memory allocated by C#

    return appdata;
}
