#include "gamewidget.h"
#include "startmenu.h"

#include <QLabel>
#include <QLayout>
#include <QKeyEvent>
#include <QDebug>

GameWidget::GameWidget(TcpClient *tcpClient, QWidget *parent) :
    QWidget(parent),
    tcpClient(tcpClient)
{}

void GameWidget::keyPressEvent(QKeyEvent *event) {
    if(event->isAutoRepeat()) {
        event->ignore();
        return;
    }
    game->keyPress(event);
}

void GameWidget::keyReleaseEvent(QKeyEvent *event) {
    if(event->isAutoRepeat()) {
        event->ignore();
        return;
    }
    game->keyRelease(event);
}

void GameWidget::restartGame(int nbPlayers, int nbViews) {
    if(nbViews == 0) nbViews = nbPlayers;
    // S'il y a autant de QGraphicsView que de joueurs -> splitscreen
    bool isMultiplayer = nbPlayers == nbViews ? false : true;
    QString terrainFileName = ":/Resources/debugTerrain.tmx";
    game = new Game(terrainFileName, nbPlayers, isMultiplayer, tcpClient);
    QBoxLayout *hlayout = new QHBoxLayout(this);

    for(int i = 0; i < nbViews; i++) {
        View *v = new View(i);
        v->setScene(game);
        hlayout->addWidget(v);
        views.append(v);
    }

    setLayout(hlayout);
    setFocusPolicy(Qt::StrongFocus);
    setFocus();
}
