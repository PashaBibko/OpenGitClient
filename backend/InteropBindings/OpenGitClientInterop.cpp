#include "OpenGitClientInterop.h"

#include "Functions.h"

void OpenGitClientInterop::BindMessageReceiverCallback(OpenGitClientMessageReceiverCallback callback) {
    OpenGitClientInterop_BindMessageReceiver(callback);
}

void OpenGitClientInterop::StartPhotino() {
    OpenGitClientInterop_StartPhotino();
}
