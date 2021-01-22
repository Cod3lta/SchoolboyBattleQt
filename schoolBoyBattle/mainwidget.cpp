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

    QString stylesheet = ""
        "QPushButton {"
            "background-color: #1b1c1e;"
            "color: lightgray;"
            "font-family: Helvetica;"
            "font-weight: bold;"
            "font-size: 12pt;"
            "padding: 7px;"
            "border-radius: 5px"
        "}"
        "QLabel {"
            "font-family: Helvetica;"
            "color: #26292d;"
            "font-size: 12pt;"
        "}"
        "QLineEdit {"
            "padding: 7px;"
            "border: none;"
            "border-radius: 5px"
        "}";

    startMenu->setStyleSheet(stylesheet);
    finishMenu->setStyleSheet(stylesheet);
    waitingRoom->setStyleSheet(stylesheet);

    addWidget(gameWidget);
    addWidget(startMenu);
    addWidget(waitingRoom);
    addWidget(finishMenu);

    setCurrentWidget(startMenu);
    connect(startMenu, &StartMenu::startLocalGame, gameWidget, &GameWidget::restartGame);
    connect(startMenu, &StartMenu::setVisibleWidget, this, &QStackedWidget::setCurrentIndex);
    connect(gameWidget, &GameWidget::setVisibleWidget, this, &QStackedWidget::setCurrentIndex);
    connect(startMenu, &StartMenu::startClient, waitingRoom, &WaitingRoom::startWaitingRoom);
    connect(waitingRoom, &WaitingRoom::setVisibleWidget, this, &QStackedWidget::setCurrentIndex);
    connect(finishMenu, &FinishMenu::setVisibleWidget, this, &QStackedWidget::setCurrentIndex);
    connect(tcpClient, &TcpClient::startGame, gameWidget, &GameWidget::restartGame);
    connect(gameWidget, &GameWidget::showWinner, finishMenu, &FinishMenu::showWinner);
    setFocusPolicy(Qt::StrongFocus);
}
