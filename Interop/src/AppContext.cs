using System;
using System.Collections.Generic;
using Photino.NET;

namespace PashaBibko.OpenGitClient;

public static class AppContext
{
    public static Dictionary<string, Action> AppCallbacks { get; } = new();
    public static PhotinoWindow Window { get; set; }
}
