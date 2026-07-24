using System;
using System.Text.RegularExpressions;

namespace PashaBibko.OpenGitClient;

public static partial class MessageHandler
{
    public static void Handle(object sender, string message)
    {
        Console.WriteLine("Message: " + message);

        // Fetches the requested function name out of the message
        Match functionNameMatch = FunctionNameRegex().Match(message);
        if (!functionNameMatch.Success)
        {
            Console.WriteLine("ERROR: Could not find function name.");
            return;
        }
        string functionName = functionNameMatch.Groups[1].Value;

        // Locates the callback using the function name
        bool hasCallback = AppContext.AppCallbacks.TryGetValue(functionName, out Action callback);
        if (!hasCallback)
        {
            Console.WriteLine($"ERROR: Could not find callback of {functionName}.");
            return;
        }
        
        // Invokes the callback
        callback.Invoke();
    }

    [GeneratedRegex(@"\[(.*?)\]")] private static partial Regex FunctionNameRegex();
}
