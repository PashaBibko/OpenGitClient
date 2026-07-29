#pragma once

#include "OpenGitClientInterop.h"

extern "C" {
    void OpenGitClientInterop_BindMessageReceiver(OpenGitClientMessageReceiverCallback);
    void OpenGitClientInterop_StartPhotino();
    void OpenGitClientInterop_FreePtr(void*);

    void* OpenGitClientInterop_GetAppDataDir();
}
