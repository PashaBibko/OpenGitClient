using System.Runtime.InteropServices;

namespace PashaBibko.OpenGitClient;

public static partial class InteropExports
{
    [UnmanagedCallersOnly(EntryPoint = "OpenGitClientInterop_StartPhotino")]
    public static void StartPhotino_MacOS()
    {
        AppContext.CreateDefaultWindowInstance()
            .Load("web/index.html")
            .RegisterWebMessageReceivedHandler(MessageHandler.Handle)
            .WaitForClose();
    }
}
