#include "InteropBindings/OpenGitClientInterop.h"
#include "Logger/LoggerWebFunctions.h"
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

    router.AddFunction<ExternalLoggerInterface::Log>("App.Log");
    router.AddFunction<ExternalLoggerInterface::LogError>("App.LogError");

    router.AddFunction<Repo::Choose>("Repo.Choose");
    router.AddFunction<Repo::Open>("Repo.Open");
    router.AddFunction<Repo::GetChanges>("Repo.GetChanges");
    router.AddFunction<Repo::GetFileDiff>("Repo.GetFileDiff");

    router.AddFunction<User::GetLastOpenedRepo>("User.GetLastOpenedRepo");

    // Starts photino.net with a message receiver to forward to the web function router
    {
        static char* sharedBuffer = nullptr;
        OpenGitClientInterop::BindMessageReceiverCallback(
            [](const char* name, const char* serializedVal) -> char* {
                const std::optional result = router.InvokeFunction(name, serializedVal);
                if (!result.has_value()) {
                    return nullptr; // Will be converted to "null" by C#
                }

                // Deletes anything that may have been stored in the shared buffer, the
                // buffer being shared is fine because it works in a single thread, it will technically leak some
                // memory at the end of the process, but ohh well, if you are looking at this code to consider if
                // you should hire me please ignore the fact that I am leaking memory here. Please hire me thx.
                delete[] sharedBuffer;
                sharedBuffer = nullptr;

                // Copies the string to a cstr so it can be passed to C#
                const std::string& serialized = result.value();
                const size_t serializedLen = serialized.size();
                sharedBuffer = new char[serializedLen + 1];
                std::ranges::copy(serialized, sharedBuffer);
                sharedBuffer[serializedLen] = '\0';

                return sharedBuffer;
            });
    }
    OpenGitClientInterop::StartPhotino(); // Blocks until window is closed
}
