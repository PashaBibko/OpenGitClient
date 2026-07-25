using System.Runtime.InteropServices;

using TextEncoding = System.Text.Encoding;

namespace PashaBibko.OpenGitClient;

public static partial class InteropExports
{
    [UnmanagedCallersOnly(EntryPoint = "OpenGitClientInterop_BindMessageReceiver")]
    public static unsafe void BindMessageReceiver(delegate* unmanaged[Cdecl]<byte*, byte*, void> function)
    {
        AppContext.MessageReceivedCallback = (func, obj) =>
        {
            fixed (byte* funcBytesPtr = TextEncoding.UTF8.GetBytes(func))
            fixed (byte* objBytesPtr = obj == null ? null : TextEncoding.UTF8.GetBytes(obj))
            {
                function(funcBytesPtr, objBytesPtr);
            }
        };
    }
}