#pragma once

using OpenGitClientCallback = void (*)();

class OpenGitClientInterop {
public:
    // Static class, not meant to be created / destroyed
    OpenGitClientInterop() = delete;
    ~OpenGitClientInterop() = delete;

    // Function bindings
    static void BindCallback(const char* name, OpenGitClientCallback callback);
    static void StartPhotino();
};
