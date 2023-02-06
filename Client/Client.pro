QT       += core gui network sql #axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
LIBS += -lpthread libwsock32 libws2_32


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
../../depends/CELLBuffer.cpp \
../../depends/CELLClient.cpp \
../../depends/CELLLog.cpp \
../../depends/CELLMsgStream.cpp \
../../depends/CELLServer.cpp \
../../depends/CELLStream.cpp \
../../depends/CELLTask.cpp \
../../depends/EasyTcpClient.cpp \
../../depends/EasyTcpServer.cpp \
../../depends/server.cpp \
../../depends/user.cpp \
    ../Server/Object/Area.cpp \
    ../Server/Object/object.cpp \
    areaview.cpp \
    cclient.cpp \
    cuser.cpp \
    loginui.cpp \
    main.cpp \
    mainview.cpp \
    qdoubleedit.cpp \
    qjsoncmd.cpp \
    regdialog.cpp \
    waitdlg.cpp

HEADERS += \
../../depends/CELL.h \
../../depends/CELLBuffer.h \
../../depends/CELLClient.h \
../../depends/CELLLog.h \
../../depends/CELLMsgStream.h \
../../depends/CELLNetWork.hpp \
../../depends/CELLSemaphore.hpp \
../../depends/CELLServer.h \
../../depends/CELLStream.h \
../../depends/CELLTask.h \
../../depends/CELLThread.h \
../../depends/CELLTimestamp.h \
../../depends/EasyTcpClient.h \
../../depends/EasyTcpServer.h \
../../depends/INetEvent.h \
../../depends/MessageHeader.h \
../../depends/user.h \
    ../Server/Object/Area.h \
    ../Server/Object/Object.h \
    areaview.h \
    cclient.h \
    cuser.h \
    loginui.h \
    mainview.h \
    qdoubleedit.h \
    qjsoncmd.h \
    regdialog.h \
    tabfactory.h \
    waitdlg.h

FORMS += \
    loginui.ui \
    mainview.ui \
    regdialog.ui \
    waitdlg.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
#INCLUDEPATH += $$PWD/../../include
#DEPENDPATH += $$PWD/../../depends

CONFIG(debug,debug|release){
    DESTDIR += ../../debug
}else{
    DESTDIR += ../../bin
}

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/ -lNetKits
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/ -lNetKitsd
#else:unix: LIBS += -L$$PWD/../../lib/ -lNetKits

#INCLUDEPATH += $$PWD/../netkits
#DEPENDPATH += $$PWD/../netkits


