QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    animateditem.cpp \
    candy.cpp \
    dataloader.cpp \
    display.cpp \
    game.cpp \
    keyinputs.cpp \
    main.cpp \
    menu.cpp \
    player.cpp \
    tile.cpp \
    tilecandyplacement.cpp \
    view.cpp \
    viewscontainer.cpp

HEADERS += \
    animateditem.h \
    candy.h \
    dataloader.h \
    display.h \
    game.h \
    keyinputs.h \
    menu.h \
    player.h \
    tile.h \
    tilecandyplacement.h \
    view.h \
    viewscontainer.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

QT += xml
