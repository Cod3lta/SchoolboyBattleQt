#include "gamewidget.h"
#include "mainwidget.h"
#include "startmenu.h"

MainWidget::MainWidget() :
    tcpClient(new TcpClient(this))
{

    gameWidget = new GameWidget(this);
    StartMenu *startMenu = new StartMenu(this);
    waitingRoom = new WaitingRoom(tcpClient, this);

    addWidget(gameWidget);
    addWidget(startMenu);
    addWidget(waitingRoom);

    setCurrentWidget(startMenu);
    connect(startMenu, &StartMenu::startLocalGame, gameWidget, &GameWidget::restartLocalGame);
    connect(startMenu, &StartMenu::setVisibleWidget, this, &QStackedWidget::setCurrentIndex);
    connect(startMenu, &StartMenu::startClient, waitingRoom, &WaitingRoom::startClient);
    connect(waitingRoom, &WaitingRoom::setVisibleWidget, this, &QStackedWidget::setCurrentIndex);
    setFocusPolicy(Qt::StrongFocus);
}
