QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    candy.cpp \
    dataloader.cpp \
    display.cpp \
    finishmenu.cpp \
    game.cpp \
    gamewidget.cpp \
    keyinputs.cpp \
    main.cpp \
    mainwidget.cpp \
    player.cpp \
    startmenu.cpp \
    tcpclient.cpp \
    tile.cpp \
    tilecandyplacement.cpp \
    view.cpp \
    waitingroom.cpp

HEADERS += \
    candy.h \
    dataloader.h \
    display.h \
    finishmenu.h \
    game.h \
    gamewidget.h \
    keyinputs.h \
    mainwidget.h \
    player.h \
    startmenu.h \
    tcpclient.h \
    tile.h \
    tilecandyplacement.h \
    view.h \
    waitingroom.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
