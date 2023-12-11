#-------------------------------------------------
#
# Project created by QtCreator 2018-07-13T09:30:13
#
#-------------------------------------------------

QT       += core gui network opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lib-communi
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += IRC_STATIC

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += .
INCLUDEPATH += ./IrcCore
INCLUDEPATH += ./IrcModel
INCLUDEPATH += ./IrcUtil

unix:MOC_DIR = ./mocs
win32:MOC_DIR = ./mocs

win32:INCLUDEPATH+=C:\Users\TEST\Downloads\irrlicht-1.8.4\irrlicht-1.8.4\include
else:unix:INCLUDEPATH+= /home/Dev/libs/game/irrlicht/Irrlicht-SVN/include/ /home/Dev/libs/game/irrlicht/Irrlicht-SVN/lib/Linux/ -Wall

win32:LIBS += -LC:\Users\TEST\Downloads\irrlicht-1.8.4\irrlicht-1.8.4\lib\Win32-visualstudio -lIrrlicht
else:unix:LIBS += -L/home/Dev/libs/game/irrlicht/Irrlicht-SVN/lib/Linux/ -LC:\Users\TEST\Downloads\irrlicht-1.8.4\irrlicht-1.8.4\lib\Win32-visualstudio   -lQt5OpenGL -lQt5Widgets -lQt5Gui -lQt5Core -lIrrlicht -lGL -lX11 -lXxf86vm -luchardet 

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    IrcCore/irc.cpp \
    IrcCore/irccommand.cpp \
    IrcCore/ircconnection.cpp \
    IrcCore/irccore.cpp \
    IrcCore/ircfilter.cpp \
    IrcCore/ircmessage.cpp \
    IrcCore/ircmessage_p.cpp \
    IrcCore/ircmessagecomposer.cpp \
    IrcCore/ircmessagedecoder.cpp \
 #   IrcCore/ircmessagedecoder_icu.cpp \
#    IrcCore/ircmessagedecoder_uchardet.cpp \
    IrcCore/ircmessagedecoder_none.cpp \
    IrcCore/ircnetwork.cpp \
    IrcCore/ircprotocol.cpp \
    IrcModel/ircbuffer.cpp \
    IrcModel/ircbuffermodel.cpp \
    IrcModel/ircchannel.cpp \
    IrcModel/ircmodel.cpp \
    IrcModel/ircuser.cpp \
    IrcModel/ircusermodel.cpp \
    IrcUtil/irccommandparser.cpp \
    IrcUtil/irccommandqueue.cpp \
    IrcUtil/irccompleter.cpp \
    IrcUtil/irclagtimer.cpp \
    IrcUtil/ircpalette.cpp \
    IrcUtil/irctextformat.cpp \
    IrcUtil/irctoken.cpp \
    IrcUtil/ircutil.cpp \
    ircclient.cpp \
    ircmessageformatter.cpp \
    irrlichtwidget.cpp \
    irrutil.cpp \
    movemodelanimator.cpp \
    irrwidget.cpp \
    ircwidget.cpp \
    irreventreciever.cpp \
    irrapplication.cpp


HEADERS += \
        mainwindow.h \
    IrcCore/Irc \
    IrcCore/irc.h \
    IrcCore/IrcCommand \
    IrcCore/irccommand.h \
    IrcCore/irccommand_p.h \
    IrcCore/IrcCommandFilter \
    IrcCore/irccommandfilter.h \
    IrcCore/IrcConnection \
    IrcCore/ircconnection.h \
    IrcCore/ircconnection_p.h \
    IrcCore/IrcCore \
    IrcCore/irccore.h \
    IrcCore/ircdebug_p.h \
    IrcCore/IrcFilter \
    IrcCore/ircfilter.h \
    IrcCore/IrcGlobal \
    IrcCore/ircglobal.h \
    IrcCore/IrcMessage \
    IrcCore/ircmessage.h \
    IrcCore/ircmessage_p.h \
    IrcCore/ircmessagecomposer_p.h \
    IrcCore/ircmessagedecoder_p.h \
    IrcCore/IrcMessageFilter \
    IrcCore/ircmessagefilter.h \
    IrcCore/IrcNetwork \
    IrcCore/ircnetwork.h \
    IrcCore/ircnetwork_p.h \
    IrcCore/IrcProtocol \
    IrcCore/ircprotocol.h \
    IrcModel/ircbuffer.h \
    IrcModel/ircbuffer_p.h \
    IrcModel/ircbuffermodel.h \
    IrcModel/ircbuffermodel_p.h \
    IrcModel/ircchannel.h \
    IrcModel/ircchannel_p.h \
    IrcModel/ircmodel.h \
    IrcModel/ircuser.h \
    IrcModel/ircuser_p.h \
    IrcModel/ircusermodel.h \
    IrcModel/ircusermodel_p.h \
    IrcModel/IrcBuffer \
    IrcModel/IrcBufferModel \
    IrcModel/IrcChannel \
    IrcModel/IrcModel \
    IrcModel/IrcUser \
    IrcModel/IrcUserModel \
    IrcUtil/IrcCommandParser \
    IrcUtil/irccommandparser.h \
    IrcUtil/irccommandparser_p.h \
    IrcUtil/IrcCommandQueue \
    IrcUtil/irccommandqueue.h \
    IrcUtil/irccommandqueue_p.h \
    IrcUtil/IrcCompleter \
    IrcUtil/irccompleter.h \
    IrcUtil/IrcLagTimer \
    IrcUtil/irclagtimer.h \
    IrcUtil/irclagtimer_p.h \
    IrcUtil/IrcPalette \
    IrcUtil/ircpalette.h \
    IrcUtil/IrcTextFormat \
    IrcUtil/irctextformat.h \
    IrcUtil/irctoken_p.h \
    IrcUtil/IrcUtil \
    IrcUtil/ircutil.h \
    ircclient.h \
    ircmessageformatter.h \
    irrlichtwidget.h \
    irrutil.h \
    movemodelanimator.h \
    irrwidget.h \
    ircwidget.h \
    irreventreciever.h \
    irrapplication.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
