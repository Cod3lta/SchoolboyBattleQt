/*
 * Parce qu'il peut y avoir plusieurs vues en même temps accédant à la même
 * scene, il faut une classe container qui créé le bon nombre de vues et
 * qui les place au bon endroit
 */

#include "viewscontainer.h"

#include <QBoxLayout>
#include <QDebug>
#include <QKeyEvent>

ViewsContainer::ViewsContainer(QWidget *parent)
    : QMainWindow(parent)
{
    game = new Game();
    mainView = new View();
    View *viewPlayer2 = new View();
    mainView->setScene(game);
    viewPlayer2->setScene(game);

    QBoxLayout *vlayout = new QHBoxLayout(this);
    QWidget *widget = new QWidget();
    widget->setLayout(vlayout);

    vlayout->addWidget(mainView);
    vlayout->addWidget(viewPlayer2);

    setCentralWidget(widget);
}

void ViewsContainer::keyPressEvent(QKeyEvent *event) {
    if(event->isAutoRepeat()) {
        event->ignore();
    }
    game->keyPress(event);
}

void ViewsContainer::keyReleaseEvent(QKeyEvent *event) {
    if(event->isAutoRepeat()) {
        event->ignore();
    }
    game->keyRelease(event);
}
