#ifndef VIEWSCONTAINER_H
#define VIEWSCONTAINER_H

#include <QMainWindow>
#include "view.h"
#include "game.h"

class ViewsContainer : public QMainWindow
{
    Q_OBJECT
public:
    ViewsContainer(int nbPlayers, QMainWindow *parent = nullptr);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    QList<QGraphicsView*> views;
    Game *game;
signals:

};

#endif // VIEWSCONTAINER_H
