using System.Runtime.InteropServices;

namespace PashaBibko.OpenGitClient;

public static class InteropExports
{
    [UnmanagedCallersOnly(EntryPoint = "OpenGitClientInterop_StartPhotino")]
    public static void StartPhotino() { }

    [UnmanagedCallersOnly(EntryPoint = "OpenGitClientInterop_TestAdd")]
    public static int TestAdd(int a, int b) => a + b;
}
