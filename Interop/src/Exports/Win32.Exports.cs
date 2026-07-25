using System.Runtime.InteropServices;
using System.Runtime.Versioning;
using System.Threading;

namespace PashaBibko.OpenGitClient;

public static partial class InteropExports
{
    [UnmanagedCallersOnly(EntryPoint = "OpenGitClientInterop_StartPhotino")]
    [SupportedOSPlatform("windows")]
    public static void StartPhotino_Win32()
    {
        Thread t = new(() =>
        {
            AppContext.CreateDefaultWindowInstance()
                .Load("web/index.html")
                .RegisterWebMessageReceivedHandler(MessageHandler.Handle)
                .WaitForClose();
        });
        
        t.SetApartmentState(ApartmentState.STA);
        t.Start();
        t.Join();
    }
}
