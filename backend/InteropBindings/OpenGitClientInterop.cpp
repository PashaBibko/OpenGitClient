#include "OpenGitClientInterop.h"

#include <cstring>

#include "Functions.h"

void OpenGitClientInterop::BindCallback(const char* name, OpenGitClientCallback callback) {
    char* nameCopy = new char[strlen(name) + 1];
    strcpy(nameCopy, name);

    OpenGitClientInterop_BindCallback(nameCopy, callback);
}

void OpenGitClientInterop::StartPhotino() {
    OpenGitClientInterop_StartPhotino();
}
