QT       += core gui network xml multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 resources_big

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    boss.cpp \
    candy.cpp \
    dataloader.cpp \
    display.cpp \
    game.cpp \
    gamewidget.cpp \
    keyinputs.cpp \
    main.cpp \
    mainwidget.cpp \
    menu.cpp \
    player.cpp \
    startmenu.cpp \
    tcpclient.cpp \
    tile.cpp \
    tilecandyplacement.cpp \
    view.cpp \
    waitingroom.cpp

HEADERS += \
    boss.h \
    candy.h \
    dataloader.h \
    display.h \
    game.h \
    gamewidget.h \
    keyinputs.h \
    mainwidget.h \
    menu.h \
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
