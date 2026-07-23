#pragma once

class OpenGitClientInterop {
public:
    // Static class, not meant to be created / destroyed
    OpenGitClientInterop() = delete;
    ~OpenGitClientInterop() = delete;

    // Function bindings
    static void StartPhotino();
};
