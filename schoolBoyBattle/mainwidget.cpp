#include "gamewidget.h"
#include "mainwidget.h"
#include "startmenu.h"
#include "finishmenu.h"

MainWidget::MainWidget() :
    tcpClient(new TcpClient(this))
{

    gameWidget = new GameWidget(tcpClient, this);
    StartMenu *startMenu = new StartMenu(this);
    FinishMenu *finishMenu = new FinishMenu(this);
    waitingRoom = new WaitingRoom(tcpClient, this);

    addWidget(gameWidget);
    addWidget(startMenu);
    addWidget(waitingRoom);
    addWidget(finishMenu);

    setCurrentWidget(startMenu);
    connect(startMenu, &StartMenu::startLocalGame, gameWidget, &GameWidget::restartGame);
    connect(startMenu, &StartMenu::setVisibleWidget, this, &QStackedWidget::setCurrentIndex);
    connect(startMenu, &StartMenu::startClient, waitingRoom, &WaitingRoom::startWaitingRoom);
    connect(waitingRoom, &WaitingRoom::setVisibleWidget, this, &QStackedWidget::setCurrentIndex);
    connect(finishMenu, &FinishMenu::setVisibleWidget, this, &QStackedWidget::setCurrentIndex);
    connect(tcpClient, &TcpClient::startGame, gameWidget, &GameWidget::restartGame);
    setFocusPolicy(Qt::StrongFocus);
}
