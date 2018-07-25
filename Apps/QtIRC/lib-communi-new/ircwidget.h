#ifndef IRCWIDGET_H
#define IRCWIDGET_H

#include <QSplitter>
#include <QHash>

class IrcBuffer;
class IrcMessage;
class IrcUserModel;
class IrcCompleter;
class IrcConnection;
class IrcBufferModel;
class IrcCommandParser;

QT_FORWARD_DECLARE_CLASS(QLineEdit);
QT_FORWARD_DECLARE_CLASS(QListView);
QT_FORWARD_DECLARE_CLASS(QTextEdit);
QT_FORWARD_DECLARE_CLASS(QModelIndex);
QT_FORWARD_DECLARE_CLASS(QTextDocument);

class IrcWidget : public QSplitter {
    Q_OBJECT
public:
    IrcWidget(QWidget* pParent = 0);
    ~IrcWidget();

private slots:
    void onConnected();
    void onConnecting();
    void onDisconnected();

    void onTextEdited();
    void onTextEntered();

    void onCompletion();
    void onCompleted(const QString& text, int cursor);

    void onBufferAdded(IrcBuffer* buffer);
    void onBufferRemoved(IrcBuffer* buffer);

    void onBufferActivated(const QModelIndex& index);
    void onUserActivated(const QModelIndex& index);

    void recieveMessage(IrcMessage* message);

private:
    void createLayout();
    void createCompleter();
    void createParser();
    void createUserList();
    void createBufferList();
    void createConnection();

    QLineEdit* m_pInputEdit;
    QTextEdit* m_pMessageText;
    QListView* m_pUserList;
    QListView* m_pBufferList;

    IrcCompleter* completer;
    IrcCommandParser* parser;
    IrcConnection* connection;
    IrcBufferModel* bufferModel;
    QHash<IrcBuffer*, IrcUserModel*> userModels;
    QHash<IrcBuffer*, QTextDocument*> documents;
};

#endif // IRCWIDGET_H
