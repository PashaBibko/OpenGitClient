using System;
using System.Runtime.InteropServices;
using Photino.NET;

namespace PashaBibko.OpenGitClient;

public static class InteropExports
{
    [UnmanagedCallersOnly(EntryPoint = "OpenGitClientInterop_BindCallback")]
    public static unsafe void BindCallback(byte* callbackName, delegate*<void> function)
    {
        string name = Marshal.PtrToStringUTF8((IntPtr)callbackName);
        if (name == null)
        {
            Console.WriteLine("ERROR: Could not marshal callback name.");
            return;
        }

        AppContext.AppCallbacks.Add(name, () => function());
    }

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
        
        AppContext.Window
            .Load("web/index.html")
            .Center()
            .RegisterWebMessageReceivedHandler(MessageHandler.Handle)
            .WaitForClose();
    }
}
