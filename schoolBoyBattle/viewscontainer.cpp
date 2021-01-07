/*
 * Parce qu'il peut y avoir plusieurs vues en même temps accédant à la même
 * scene, il faut une classe container qui créé le bon nombre de vues et
 * qui les place au bon endroit
 */

#include "viewscontainer.h"

#include <QBoxLayout>
#include <QDebug>
#include <QKeyEvent>

ViewsContainer::ViewsContainer(int nbPlayers, QMainWindow *parent)
    : QMainWindow(parent)
{
    game = new Game(nbPlayers);
    QBoxLayout *vlayout = new QHBoxLayout(this);
    QWidget *widget = new QWidget();
    widget->setLayout(vlayout);

    for(int i = 0; i < nbPlayers; i++) {
        View *v = new View(i);
        v->setScene(game);
        vlayout->addWidget(v);
        views.append(v);
    }

    setCentralWidget(widget);
}

void ViewsContainer::keyPressEvent(QKeyEvent *event) {
    if(event->isAutoRepeat()) {
        event->ignore();
        return;
    }
    game->keyPress(event);
}

void ViewsContainer::keyReleaseEvent(QKeyEvent *event) {
    if(event->isAutoRepeat()) {
        event->ignore();
        return;
    }
    game->keyRelease(event);
}
