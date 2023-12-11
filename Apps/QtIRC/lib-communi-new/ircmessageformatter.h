#ifndef IRCMESSAGEFORMATTER_H
#define IRCMESSAGEFORMATTER_H

#include <IrcMessage>

class IrcMessageFormatter
{
public:
    static QString formatMessage(IrcMessage* message);
    static QString formatMessage(const QString& message);

private:
    static QString formatJoinMessage(IrcJoinMessage* message);
    static QString formatModeMessage(IrcModeMessage* message);
    static QString formatNamesMessage(IrcNamesMessage* message);
    static QString formatNickMessage(IrcNickMessage* message);
    static QString formatPartMessage(IrcPartMessage* message);
    static QString formatPrivateMessage(IrcPrivateMessage* message);
    static QString formatQuitMessage(IrcQuitMessage* message);
};

#endif // IRCMESSAGEFORMATTER_H
