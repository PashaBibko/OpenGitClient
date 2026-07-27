using System;
using System.Runtime.InteropServices;

using TextEncoding = System.Text.Encoding;

namespace PashaBibko.OpenGitClient;

public static partial class InteropExports
{
    [UnmanagedCallersOnly(EntryPoint = "OpenGitClientInterop_BindMessageReceiver")]
    public static unsafe void BindMessageReceiver(delegate* unmanaged[Cdecl]<byte*, byte*, byte*> function)
    {
        AppContext.MessageReceivedCallback = (func, obj) =>
        {
            fixed (byte* funcBytesPtr = TextEncoding.UTF8.GetBytes(func))
            fixed (byte* objBytesPtr = obj == null ? null : TextEncoding.UTF8.GetBytes(obj))
            {
                byte* outputBytesPtr = function(funcBytesPtr, objBytesPtr);
                if (outputBytesPtr != null)
                {
                    string serializedOutput = Marshal.PtrToStringUTF8((IntPtr)outputBytesPtr);

                    Console.WriteLine($"Sent: [{serializedOutput}]");
                    AppContext.Window.SendWebMessage(serializedOutput);
                }

                else
                {
                    AppContext.Window.SendWebMessage("null");
                    Console.WriteLine("Sent: [null]");
                }
            }
        };
    }
}