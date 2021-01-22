#include "gamewidget.h"
#include "mainwidget.h"
#include "startmenu.h"
#include "finishmenu.h"

#include <QMessageBox>

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
    connect(startMenu, &StartMenu::startLocalGame, gameWidget, &GameWidget::startGame);
    connect(startMenu, &StartMenu::setVisibleWidget, this, &QStackedWidget::setCurrentIndex);
    connect(startMenu, &StartMenu::startClient, waitingRoom, &WaitingRoom::startWaitingRoom);
    connect(waitingRoom, &WaitingRoom::setVisibleWidget, this, &QStackedWidget::setCurrentIndex);
    connect(gameWidget, &GameWidget::setVisibleWidget, this, &QStackedWidget::setCurrentIndex);
    connect(gameWidget, &GameWidget::setFinishMenuWinner, finishMenu, &FinishMenu::showWinner);
    connect(finishMenu, &FinishMenu::setVisibleWidget, this, &QStackedWidget::setCurrentIndex);
    connect(finishMenu, &FinishMenu::resetGame, gameWidget, &GameWidget::resetGame);
    connect(tcpClient, &TcpClient::startGame, gameWidget, &GameWidget::startGame);
    connect(tcpClient, &TcpClient::connectionError, this, [=] () {
        setCurrentIndex(1);
    });
    connect(tcpClient, &TcpClient::disconnected, this, [=] () {
        gameWidget->resetGame();
        setCurrentIndex(1);
        QMessageBox::critical(nullptr, "Erreur", "Déconnecté du serveur");
    });
    setFocusPolicy(Qt::StrongFocus);
}
