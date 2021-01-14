#include "gamewidget.h"
#include "startmenu.h"

#include <QLabel>
#include <QLayout>
#include <QKeyEvent>

GameWidget::GameWidget(QWidget *parent) : QWidget(parent)
{
    int nbPlayers = 2;
    QString terrainFileName = ":/Resources/debugTerrain.tmx";
    game = new Game(nbPlayers, terrainFileName);
    QBoxLayout *hlayout = new QHBoxLayout(this);

    for(int i = 0; i < nbPlayers; i++) {
        View *v = new View(i);
        v->setScene(game);
        hlayout->addWidget(v);
        views.append(v);
    }

    setLayout(hlayout);
}

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

void GameWidget::createLocalGame()
{
}
