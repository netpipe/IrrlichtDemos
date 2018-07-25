#include "ircwidget.h"

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

static const char* CHANNEL = "#freenode";
static const char* SERVER = "irc.freenode.net";

IrcWidget::IrcWidget(QWidget* parent)
    : QSplitter (parent){
    createParser();
    createConnection();
    createCompleter();
    createUserList();
    createLayout();
    createBufferList();

    connection->sendCommand(IrcCommand::createJoin(CHANNEL));
    connection->open();

    m_pMessageText->append(IrcMessageFormatter::formatMessage(tr("! Welcome to the Communi %1 example client.").arg(IRC_VERSION_STR)));
    m_pMessageText->append(IrcMessageFormatter::formatMessage(tr("! This example connects %1 and joins %2.").arg(SERVER, CHANNEL)));
    m_pMessageText->append(IrcMessageFormatter::formatMessage(tr("! PS. Available commands: JOIN, ME, NICK, PART")));
}

IrcWidget::~IrcWidget(){
    if(connection->isActive()){
        connection->quit(connection->realName());
        connection->close();
    }
}

void IrcWidget::onConnected(){
    m_pMessageText->append(IrcMessageFormatter::formatMessage("! Connected to %1.").arg(SERVER));
    m_pMessageText->append(IrcMessageFormatter::formatMessage("! Joinning %1...").arg(CHANNEL));
}

void IrcWidget::onConnecting(){
    m_pMessageText->append(IrcMessageFormatter::formatMessage("! Connecting %1...").arg(SERVER));
}

void IrcWidget::onDisconnected(){
    m_pMessageText->append(IrcMessageFormatter::formatMessage("! Disconnected %1...").arg(SERVER));
}

void IrcWidget::onTextEdited(){
    m_pInputEdit->setStyleSheet(QString());
}

void IrcWidget::onTextEntered(){
    QString input = m_pInputEdit->text();
    IrcCommand* command = parser->parse(input);
    if(command){
        connection->sendCommand(command);
        if(command->type() == IrcCommand::Message || command->type() == IrcCommand::CtcpAction){
            IrcMessage* msg = command->toMessage(connection->nickName(), connection);
            recieveMessage(msg);
            delete msg;
        }
        m_pInputEdit->clear();
    }else if(input.length() > 1){
        QString error;
        QString command = m_pInputEdit->text().mid(1).split(" ", QString::SkipEmptyParts).value(0).toUpper();
        if(parser->commands().contains(command)){
            error = tr("[ERROR] Syntax: %1").arg(parser->syntax(command).replace("<", "&lt;").replace(">", "&gt;"));
        }else {
            error = tr("[ERROR] Unknown Command: %1").arg(command);
        }
        m_pMessageText->append(IrcMessageFormatter::formatMessage(error));
        m_pInputEdit->setStyleSheet("background: salmon");
    }
}

void IrcWidget::onCompletion(){
    completer->complete(m_pInputEdit->text(), m_pInputEdit->cursorPosition());
}

void IrcWidget::onCompleted(const QString& text, int cursor){
    m_pInputEdit->setText(text);
    m_pInputEdit->setCursorPosition(cursor);
}

void IrcWidget::onBufferAdded(IrcBuffer* buffer){
    connect(buffer, SIGNAL(messageRecieved(IrcMessage*)), this, SLOT(recieveMessage(IrcMessage*)));

    QTextDocument* document = new QTextDocument(buffer);
    documents.insert(buffer, document);

    IrcUserModel* userModel = new IrcUserModel(buffer);
    userModel->setSortMethod(Irc::SortByTitle);
    userModels.insert(buffer, userModel);

    int index = bufferModel->buffers().indexOf(buffer);
    if(index != -1){
        m_pBufferList->setCurrentIndex(bufferModel->index(index));
    }
}

void IrcWidget::onBufferRemoved(IrcBuffer* buffer){
    delete userModels.take(buffer);
    delete documents.take(buffer);
}

void IrcWidget::onBufferActivated(const QModelIndex& index){
    IrcBuffer* buffer = index.data(Irc::BufferRole).value<IrcBuffer*>();

    m_pMessageText->setDocument(documents.value(buffer));
    m_pMessageText->verticalScrollBar()->triggerAction(QScrollBar::SliderToMaximum);
    m_pUserList->setModel(userModels.value(buffer));

    if(buffer){
        parser->setTarget(buffer->title());
    }
}

void IrcWidget::onUserActivated(const QModelIndex& index){
    IrcUser* user = index.data(Irc::UserRole).value<IrcUser*>();
    if(user){
        IrcBuffer* buffer = bufferModel->add(user->name());

        int index = bufferModel->buffers().indexOf(buffer);
        if(index != -1){
            m_pBufferList->setCurrentIndex(bufferModel->index(index));
        }
    }
}

static void appendHtml(QTextDocument* document, const QString& html){
    QTextCursor cursor(document);
    cursor.beginEditBlock();
    cursor.movePosition(QTextCursor::End);
    if(!document->isEmpty()){
        cursor.insertBlock();
    }
    cursor.insertHtml(html);
    cursor.endEditBlock();
}

void IrcWidget::recieveMessage(IrcMessage* message){
    IrcBuffer* buffer = qobject_cast<IrcBuffer*>(sender());
    if(!buffer){
        buffer = m_pBufferList->currentIndex().data(Irc::BufferRole).value<IrcBuffer*>();
    }
    QTextDocument* document = documents.value(buffer);
    if(document){
        QString html = IrcMessageFormatter::formatMessage(message);
        if(!html.isEmpty()){
            m_pMessageText->append(html);
        }else{
            appendHtml(document, html);
        }
    }
}

void IrcWidget::createLayout(){
    setWindowTitle(tr("Communi %1 example client").arg(IRC_VERSION_STR));

    m_pMessageText = new QTextEdit(this);
    m_pMessageText->setReadOnly(true);

    m_pInputEdit = new QLineEdit(this);
    m_pInputEdit->setAttribute(Qt::WA_MacShowFocusRect, false);
    m_pMessageText->setFocusProxy(m_pInputEdit);
    connect(m_pInputEdit, SIGNAL(returnPressed()), this, SLOT(onTextEntered()));
    connect(m_pInputEdit, SIGNAL(textEdited(QString)), this, SLOT(onTextEdited()));

    QSplitter* splitter = new QSplitter(this);
    splitter->setHandleWidth(1);
    splitter->addWidget(m_pMessageText);
    splitter->addWidget(m_pUserList);
    splitter->setStretchFactor(0, 5);
    splitter->setStretchFactor(1, 1);

    QWidget* container = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(container);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(splitter);
    layout->addWidget(m_pInputEdit);

    addWidget(container);
    setHandleWidth(1);
}

void IrcWidget::createCompleter(){
    completer = new IrcCompleter();
    completer->setParser(parser);
    connect(completer, SIGNAL(completed(QString, int)), this, SLOT(onCompleted(QString, int)));

    QShortcut* shortcut = new QShortcut(Qt::Key_Tab, this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(onCompletion()));
}

void IrcWidget::createParser(){
    parser = new IrcCommandParser(this);
    parser->setTolerant(true);
    parser->setTriggers(QStringList("/"));
    parser->addCommand(IrcCommand::Join, "JOIN <#channel> (<key>)");
    parser->addCommand(IrcCommand::CtcpAction, "ME [target] <message...>");
    parser->addCommand(IrcCommand::Mode, "MODE (<channel/user>) (<mode>) (<arg>)");
    parser->addCommand(IrcCommand::Nick, "NICK <nick>");
    parser->addCommand(IrcCommand::Part, "PART (<#channel>) (<message...>)");
}

void IrcWidget::createUserList(){
    m_pUserList = new QListView(this);
    m_pUserList->setFocusPolicy(Qt::NoFocus);

    connect(m_pUserList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onUserActivated(QModelIndex)));
}

void IrcWidget::createBufferList(){
    bufferModel = new IrcBufferModel(connection);
    connect(bufferModel, SIGNAL(added(IrcBuffer*)), this, SLOT(onBufferAdded(IrcBuffer*)));
    connect(bufferModel, SIGNAL(removed(IrcBuffer*)), this, SLOT(onBufferRemoved(IrcBuffer*)));

    m_pBufferList = new QListView(this);
    m_pBufferList->setFocusPolicy(Qt::NoFocus);
    m_pBufferList->setModel(bufferModel);

    connect(bufferModel, SIGNAL(channelsChanged(QStringList)), parser, SLOT(setChannels(QStringList)));
    connect(m_pBufferList->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)), this, SLOT(onBufferActivated(QModelIndex)));

    IrcBuffer* serverBuffer = bufferModel->add(connection->host());
    connect(bufferModel, SIGNAL(messageIgnored(IrcMessage*)), serverBuffer, SLOT(recieveMessage(IrcMessage*)));

    insertWidget(0, m_pBufferList);
    setStretchFactor(0, 1);
    setStretchFactor(1, 3);
}

void IrcWidget::createConnection(){
    connection = new IrcConnection(this);
    connect(connection, SIGNAL(connected()), this, SLOT(onConneected()));
    connect(connection, SIGNAL(connecting()), this, SLOT(onConnecting()));
    connect(connection, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    qsrand(QTime::currentTime().msec());

    connection->setHost(SERVER);
    connection->setUserName("communi");
    connection->setNickName(tr("Client%1").arg(qrand() % 9999));
    connection->setRealName(tr("Communi %1 example client").arg(IRC_VERSION_STR));
}
