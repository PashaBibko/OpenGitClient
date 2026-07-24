using System;

namespace PashaBibko.OpenGitClient;

public static class MessageHandler
{
    public static void Handle(object sender, string message)
    {
        bool hasCallback = AppContext.AppCallbacks.TryGetValue(message, out Action callback);
        if (!hasCallback)
        {
            Console.WriteLine($"ERROR: Could not find callback of {message}");
            return;
        }
        
        callback.Invoke();
    }
}
