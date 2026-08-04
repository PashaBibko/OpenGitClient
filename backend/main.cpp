#include "InteropBindings/OpenGitClientInterop.h"
#include "AppContext/AppContextAccess.h"
#include "Router/WebFunctionRouter.h"
#include "Router/WebFunction.h"
#include "Functions/WebFunctions.h"

int main() {
    // Sets up the web function router so the messages from the frontend call the correct functions
    const std::string appdataFolder = OpenGitClientInterop::GetAppDataDir();
    static WebFunctionRouter router { appdataFolder }; // Has to be static to be passed into the callback
    router.m_AppContext.Log("Using app data folder: [", appdataFolder, ']');

    router.AddFunction<AppContextAccess::ExtLogError>("App.LogError");
    router.AddFunction<AppContextAccess::ExtLog>("App.Log");

    router.AddFunction<Repo::Choose>("Repo.Choose");
    router.AddFunction<Repo::Open>("Repo.Open");
    router.AddFunction<Repo::GetChanges>("Repo.GetChanges");
    router.AddFunction<Repo::GetFileDiff>("Repo.GetFileDiff");

    router.AddFunction<User::GetLastOpenedRepo>("User.GetLastOpenedRepo");

    // Binds the web function router before starting the main loop
    OpenGitClientInterop::BindMessageReceiverCallback(WebFunctionRouter::Route);
    OpenGitClientInterop::StartPhotino(); // Blocks until window is closed
}
