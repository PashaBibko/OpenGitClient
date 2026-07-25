using System.Runtime.InteropServices;
using Photino.NET;

using TextEncoding = System.Text.Encoding;

namespace PashaBibko.OpenGitClient;

public static class InteropExports
{
    [UnmanagedCallersOnly(EntryPoint = "OpenGitClientInterop_BindMessageReceiver")]
    public static unsafe void BindMessageReceiver(delegate* unmanaged[Cdecl]<byte*, byte*, void> function)
    {
        AppContext.MessageReceivedCallback = (funcName, serializedObject) =>
        {
            fixed (byte* pFuncName = TextEncoding.UTF8.GetBytes(funcName))
            fixed (byte* pSerializedObject = serializedObject == null
                       ? null
                       : TextEncoding.UTF8.GetBytes(serializedObject))
            {
                function(pFuncName, pSerializedObject);
            }
        };
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

internal static class MessageHandler
{
    public static void Handle(object _, string message)
    {
        string[] parts = message.Split('|', 2);
        AppContext.MessageReceivedCallback
        (
            parts[0],
            parts.Length > 1 ? parts[1] : null
        );
    }
}
