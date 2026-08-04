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
    static WebFunctionRouter router { appdataFolder }; // Has to be static to be passed into the callback

    router.AddFunction<ExternalLoggerInterface::Log>("App.Log");
    router.AddFunction<ExternalLoggerInterface::LogError>("App.LogError");

    router.AddFunction<Repo::Choose>("Repo.Choose");
    router.AddFunction<Repo::Open>("Repo.Open");
    router.AddFunction<Repo::GetChanges>("Repo.GetChanges");
    router.AddFunction<Repo::GetFileDiff>("Repo.GetFileDiff");

    router.AddFunction<User::GetLastOpenedRepo>("User.GetLastOpenedRepo");

    // Binds the web function router before starting the main loop
    OpenGitClientInterop::BindMessageReceiverCallback(WebFunctionRouter::Route);
    OpenGitClientInterop::StartPhotino(); // Blocks until window is closed
}
