using System.Runtime.InteropServices;
using Photino.NET;

namespace PashaBibko.OpenGitClient;

public static class InteropExports
{
    [UnmanagedCallersOnly(EntryPoint = "OpenGitClientInterop_StartPhotino")]
    public static void StartPhotino()
    {
        AppContext.Window = new PhotinoWindow
        {
            Title = "OpenGitClient",
            UseOsDefaultSize = true,
            Resizable = true,
            FileSystemAccessEnabled = true
        };
        
        AppContext.Window.Load("web/index.html");
        AppContext.Window.WaitForClose(); // Blocks this thread
    }
}
