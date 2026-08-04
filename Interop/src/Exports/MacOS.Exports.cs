using System;
using System.IO;
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

    [UnmanagedCallersOnly(EntryPoint = "OpenGitClientInterop_GetAppDataDir")]
    public static IntPtr GetAppDataDir_MacOS()
    {
        string homePath = Environment.GetFolderPath(Environment.SpecialFolder.UserProfile);
        string fullPath = Path.Combine(homePath, "Library", "Application Support", "OpenGitClient");

        Directory.CreateDirectory(fullPath); // Makes sure to create the dir before returning
        return Marshal.StringToHGlobalAnsi(fullPath);
    }
}
