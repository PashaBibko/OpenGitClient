#include "Functions.h"

#include <string>

#include <nfd.h>

std::string RepoLocation::Choose::Invoke(int& ctx) {
    //
    ctx++;
    std::cout << "Counter: " << ctx << "\n";

    // Initializes NFD so it can open a native file explorer window
    std::string result = "";
    if (NFD_Init() != NFD_OKAY) {
        std::cout << "ERROR: NFD_Init() failed [" << NFD_GetError() << "\n";
        return result;
    }

    // Opens the window and gets the user to select a folder
    nfdchar_t* outPath = nullptr;
    if (const nfdresult_t rc = NFD_PickFolder(&outPath, nullptr); rc == NFD_OKAY) { // nullptr = no default location
        result = outPath;
    } else if (rc != NFD_CANCEL) { // Doesn't class user canceling as failing
        std::cout << "ERROR: NFD_PickFolder() failed [" << NFD_GetError() << "]\n";
    }

    // Cleans up all allocated resources before returning
    if (outPath != nullptr) {
        NFD_FreePath(outPath);
        outPath = nullptr;
    }
    NFD_Quit();
    return result;
}