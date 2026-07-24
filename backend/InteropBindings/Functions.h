#pragma once

#include "OpenGitClientInterop.h"

extern "C" {
    void OpenGitClientInterop_BindCallback(void*, OpenGitClientCallback);
    void OpenGitClientInterop_StartPhotino();
}
