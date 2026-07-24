using System.Runtime.InteropServices;
using Photino.NET;

namespace PashaBibko.OpenGitClient;

public static class InteropExports
{
    [UnmanagedCallersOnly(EntryPoint = "OpenGitClientInterop_BindMessageReceiver")]
    public static unsafe void BindMessageReceiver(delegate* unmanaged[Cdecl]<byte*, byte*, void> function)
    {
        AppContext.MessageReceivedCallback = (funcName, serializeObject) =>
        {
            byte[] funcNameBytes = System.Text.Encoding.UTF8.GetBytes(funcName);
            byte[] serializedObjectBytes = System.Text.Encoding.UTF8.GetBytes(serializeObject);

            fixed (byte* pFuncName = funcNameBytes)
            fixed (byte* pSerializedObject = serializedObjectBytes)
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
