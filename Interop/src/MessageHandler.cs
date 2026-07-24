namespace PashaBibko.OpenGitClient;

public static class MessageHandler
{
    public static void Handle(object _, string message)
    {
        string[] parts = message.Split('|', 2);

        string function = parts[0];
        string serialized = parts.Length > 1 ? parts[1] : null;
        
        AppContext.MessageReceivedCallback(function, serialized);
    }
}
