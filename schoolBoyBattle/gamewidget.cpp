#include "gamewidget.h"
#include "startmenu.h"

#include <QLabel>
#include <QLayout>
#include <QKeyEvent>
#include <QDebug>

GameWidget::GameWidget(QWidget *parent) : QWidget(parent)
{
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

void GameWidget::restartLocalGame(int nbPlayers) {
    QString terrainFileName = ":/Resources/debugTerrain.tmx";
    game = new Game(terrainFileName);
    QBoxLayout *hlayout = new QHBoxLayout(this);

    for(int i = 0; i < nbPlayers; i++) {
        View *v = new View(i);
        v->setScene(game);
        hlayout->addWidget(v);
        views.append(v);
    }

    qDebug() << "Has focus ? " << hasFocus();

    setLayout(hlayout);
    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    qDebug() << "Has focus ? " << hasFocus();
}
