namespace PashaBibko.OpenGitClient;

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
