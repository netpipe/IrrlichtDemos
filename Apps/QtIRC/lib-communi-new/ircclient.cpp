/*
 * Copyright (C) 2008-2016 The Communi Project
 *
 * This example is free, and not covered by the BSD license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

#include "ircclient.h"
#include "ircmessageformatter.h"

#include <QTextDocument>
#include <QTextCursor>
#include <QVBoxLayout>
#include <QScrollBar>
#include <QLineEdit>
#include <QShortcut>
#include <QListView>
#include <QTextEdit>
#include <QTime>

#include <Irc>
#include <IrcUser>
#include <IrcBuffer>
#include <IrcCommand>
#include <IrcMessage>
#include <IrcUserModel>
#include <IrcCompleter>
#include <IrcConnection>
#include <IrcBufferModel>
#include <IrcCommandParser>

static const char* CHANNEL = "#freenode-test";
static const char* SERVER = "irc.freenode.net";

IrcClient::IrcClient(QWidget* parent) : QSplitter(parent)
{
    createParser();
    createConnection();
    createCompleter();
    createUserList();
    createLayout();
    createBufferList();

    // queue a command to automatically join the channel when connected
    connection->sendCommand(IrcCommand::createJoin(CHANNEL));
    connection->open();

    messageText->append(IrcMessageFormatter::formatMessage(tr("! Welcome to the Communi %1 example client.").arg(IRC_VERSION_STR)));
    messageText->append(IrcMessageFormatter::formatMessage(tr("! This example connects %1 and joins %2.").arg(SERVER, CHANNEL)));
    messageText->append(IrcMessageFormatter::formatMessage(tr("! PS. Available commands: JOIN, ME, NICK, PART")));
}

IrcClient::~IrcClient()
{
    if (connection->isActive()) {
        connection->quit(connection->realName());
        connection->close();
    }
}

void IrcClient::onConnected()
{
    messageText->append(IrcMessageFormatter::formatMessage("! Connected to %1.").arg(SERVER));
    messageText->append(IrcMessageFormatter::formatMessage("! Joining %1...").arg(CHANNEL));
}

void IrcClient::onConnecting()
{
    messageText->append(IrcMessageFormatter::formatMessage("! Connecting %1...").arg(SERVER));
}

void IrcClient::onDisconnected()
{
    messageText->append(IrcMessageFormatter::formatMessage("! Disconnected from %1.").arg(SERVER));
}

void IrcClient::onTextEdited()
{
    // clear the possible error indication
    inputEdit->setStyleSheet(QString());
}

void IrcClient::onTextEntered()
{
    QString input = inputEdit->text();
    IrcCommand* command = parser->parse(input);
    if (command) {
        connection->sendCommand(command);

        // echo own messages (servers do not send our own messages back)
        if (command->type() == IrcCommand::Message || command->type() == IrcCommand::CtcpAction) {
            IrcMessage* msg = command->toMessage(connection->nickName(), connection);
            receiveMessage(msg);
            delete msg;
        }

        inputEdit->clear();
    } else if (input.length() > 1) {
        QString error;
        QString command = inputEdit->text().mid(1).split(" ", QString::SkipEmptyParts).value(0).toUpper();
        if (parser->commands().contains(command))
            error = tr("[ERROR] Syntax: %1").arg(parser->syntax(command).replace("<", "&lt;").replace(">", "&gt;"));
        else
            error = tr("[ERROR] Unknown command: %1").arg(command);
        messageText->append(IrcMessageFormatter::formatMessage(error));
        inputEdit->setStyleSheet("background: salmon");
    }
}

void IrcClient::onCompletion()
{
    completer->complete(inputEdit->text(), inputEdit->cursorPosition());
}

void IrcClient::onCompleted(const QString& text, int cursor)
{
    inputEdit->setText(text);
    inputEdit->setCursorPosition(cursor);
}

void IrcClient::onBufferAdded(IrcBuffer* buffer)
{
    // joined a buffer - start listening to buffer specific messages
    connect(buffer, SIGNAL(messageReceived(IrcMessage*)), this, SLOT(receiveMessage(IrcMessage*)));

    // create a document for storing the buffer specific messages
    QTextDocument* document = new QTextDocument(buffer);
    documents.insert(buffer, document);

    // create a sorted model for buffer users
    IrcUserModel* userModel = new IrcUserModel(buffer);
    userModel->setSortMethod(Irc::SortByTitle);
    userModels.insert(buffer, userModel);

    // activate the new buffer
    int idx = bufferModel->buffers().indexOf(buffer);
    if (idx != -1)
        bufferList->setCurrentIndex(bufferModel->index(idx));
}

void IrcClient::onBufferRemoved(IrcBuffer* buffer)
{
    // the buffer specific models and documents are no longer needed
    delete userModels.take(buffer);
    delete documents.take(buffer);
}

void IrcClient::onBufferActivated(const QModelIndex& index)
{
    qDebug() << "buffer activated";
    IrcBuffer* buffer = index.data(Irc::BufferRole).value<IrcBuffer*>();

    // document, user list and nick completion for the current buffer
    messageText->setDocument(documents.value(buffer));
    messageText->verticalScrollBar()->triggerAction(QScrollBar::SliderToMaximum);
    userList->setModel(userModels.value(buffer));
    completer->setBuffer(buffer);

    // keep the command parser aware of the context
    if (buffer)
        parser->setTarget(buffer->title());
}

void IrcClient::onUserActivated(const QModelIndex& index)
{
    qDebug() << "User activated";
    IrcUser* user = index.data(Irc::UserRole).value<IrcUser*>();

    if (user) {
        IrcBuffer* buffer = bufferModel->add(user->name());

        // activate the new query
        int idx = bufferModel->buffers().indexOf(buffer);
        if (idx != -1)
            bufferList->setCurrentIndex(bufferModel->index(idx));
    }
}

static void appendHtml(QTextDocument* document, const QString& html)
{
    QTextCursor cursor(document);
    cursor.beginEditBlock();
    cursor.movePosition(QTextCursor::End);
    if (!document->isEmpty())
        cursor.insertBlock();
    cursor.insertHtml(html);
    cursor.endEditBlock();
}

void IrcClient::receiveMessage(IrcMessage* message)
{
    IrcBuffer* buffer = qobject_cast<IrcBuffer*>(sender());
    if (!buffer)
        buffer = bufferList->currentIndex().data(Irc::BufferRole).value<IrcBuffer*>();

    QTextDocument* document = documents.value(buffer);
    if (document) {
        QString html = IrcMessageFormatter::formatMessage(message);
        if (!html.isEmpty()) {
            if (document == messageText->document())
                messageText->append(html);
            else
                appendHtml(document, html);
        }
    }
}

void IrcClient::createLayout()
{
    setWindowTitle(tr("Communi %1 example client").arg(IRC_VERSION_STR));

    // a read-only text editor for showing the messages
    messageText = new QTextEdit(this);
    messageText->setReadOnly(true);

    // a line editor for entering commands
    inputEdit = new QLineEdit(this);
    inputEdit->setAttribute(Qt::WA_MacShowFocusRect, false);
    messageText->setFocusProxy(inputEdit);
    connect(inputEdit, SIGNAL(returnPressed()), this, SLOT(onTextEntered()));
    connect(inputEdit, SIGNAL(textEdited(QString)), this, SLOT(onTextEdited()));

    // the rest is just setting up the UI layout...
    QSplitter* splitter = new QSplitter(this);
    splitter->setHandleWidth(1);
    splitter->addWidget(messageText);
    splitter->addWidget(userList);
    splitter->setStretchFactor(0, 5);
    splitter->setStretchFactor(1, 1);

    QWidget* container = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(container);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(splitter);
    layout->addWidget(inputEdit);

    addWidget(container);

    setHandleWidth(1);
}

void IrcClient::createCompleter()
{
    // nick name completion
    completer = new IrcCompleter(this);
    completer->setParser(parser);
    connect(completer, SIGNAL(completed(QString,int)), this, SLOT(onCompleted(QString,int)));

    QShortcut* shortcut = new QShortcut(Qt::Key_Tab, this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(onCompletion()));
}

void IrcClient::createParser()
{
    // create a command parser and teach it some commands. notice also
    // that we must keep the command parser aware of the context in
    // createUi() and onBufferActivated()

    parser = new IrcCommandParser(this);
    parser->setTolerant(true);
    parser->setTriggers(QStringList("/"));
    parser->addCommand(IrcCommand::Join, "JOIN <#channel> (<key>)");
    parser->addCommand(IrcCommand::CtcpAction, "ME [target] <message...>");
    parser->addCommand(IrcCommand::Mode, "MODE (<channel/user>) (<mode>) (<arg>)");
    parser->addCommand(IrcCommand::Nick, "NICK <nick>");
    parser->addCommand(IrcCommand::Part, "PART (<#channel>) (<message...>)");
}

void IrcClient::createUserList()
{
    // a list of channel users
    userList = new QListView(this);
    userList->setFocusPolicy(Qt::NoFocus);

    // open a private query when double clicking a user
    connect(userList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onUserActivated(QModelIndex)));
}

void IrcClient::createBufferList()
{
    bufferModel = new IrcBufferModel(connection);
    connect(bufferModel, SIGNAL(added(IrcBuffer*)), this, SLOT(onBufferAdded(IrcBuffer*)));
    connect(bufferModel, SIGNAL(removed(IrcBuffer*)), this, SLOT(onBufferRemoved(IrcBuffer*)));

    bufferList = new QListView(this);
    bufferList->setFocusPolicy(Qt::NoFocus);
    bufferList->setModel(bufferModel);

    // keep the command parser aware of the context
    connect(bufferModel, SIGNAL(channelsChanged(QStringList)), parser, SLOT(setChannels(QStringList)));

    // keep track of the current buffer, see also onBufferActivated()
    connect(bufferList->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(onBufferActivated(QModelIndex)));

    // create a server buffer for non-targeted messages...
    IrcBuffer* serverBuffer = bufferModel->add(connection->host());

    // ...and connect it to IrcBufferModel::messageIgnored()
    connect(bufferModel, SIGNAL(messageIgnored(IrcMessage*)), serverBuffer, SLOT(receiveMessage(IrcMessage*)));

    insertWidget(0, bufferList);

    setStretchFactor(0, 1);
    setStretchFactor(1, 3);
}

void IrcClient::createConnection()
{
    connection = new IrcConnection(this);
    connect(connection, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(connection, SIGNAL(connecting()), this, SLOT(onConnecting()));
    connect(connection, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    qsrand(QTime::currentTime().msec());

    connection->setHost(SERVER);
    connection->setUserName("communi");
    connection->setNickName(tr("Client%1").arg(qrand() % 9999));
    connection->setRealName(tr("Communi %1 example client").arg(IRC_VERSION_STR));
}
