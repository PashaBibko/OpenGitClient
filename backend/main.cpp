#include "InteropBindings/OpenGitClientInterop.h"
#include "Router/WebFunctionRouter.h"
#include "Router/WebFunction.h"
#include "Functions.h"
#include "AppContext.h"

int main() {
    // Loads the user's appdata
    const std::string appdataFolder = OpenGitClientInterop::GetAppDataDir();
    std::cout << "Using app data folder: [" << appdataFolder << "]\n";

    // Sets up the web function router so the messages from the frontend call the correct functions
    static WebFunctionRouter<AppContext> router { appdataFolder }; // Has to be static to be passed into the callback
    router.AddFunction<Repo::Choose>("Repo.Choose");
    router.AddFunction<Repo::GetChanges>("Repo.GetChanges");
    router.AddFunction<Repo::GetFileDiff>("Repo.GetFileDiff");

    // Starts photino.net with a message receiver to forward to the web function router
    OpenGitClientInterop::BindMessageReceiverCallback(
        [](const char* name, const char* serializedVal) -> char* {
            const std::optional result = router.InvokeFunction(name, serializedVal);
            if (!result.has_value()) {
                return nullptr; // Will be converted to "null" by C#
            }

            // Copies the string to a cstr so it can be passed to C#
            // TODO: Add a way for C#/TypeScript to free to avoid memory leaking
            const std::string& serialized = result.value();
            const size_t serializedLen = serialized.size();
            char* buffer = new char[serializedLen + 1];
            std::ranges::copy(serialized, buffer);
            buffer[serializedLen] = '\0';

            return buffer;
        });
    OpenGitClientInterop::StartPhotino(); // Blocks until window is closed
}
