#pragma once

using OpenGitClientMessageReceiverCallback = char*(*)(const char*, const char*);

class OpenGitClientInterop {
public:
    // Static class, not meant to be created / destroyed
    OpenGitClientInterop() = delete;
    ~OpenGitClientInterop() = delete;

    // Function bindings
    static void BindMessageReceiverCallback(OpenGitClientMessageReceiverCallback callback);
    static void StartPhotino();
};
