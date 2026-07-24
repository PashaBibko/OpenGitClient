using System;
using Photino.NET;

namespace PashaBibko.OpenGitClient;

public static class AppContext
{
    public static Action<string, string> MessageReceivedCallback { get; set; }
    public static PhotinoWindow Window { get; set; }
}
