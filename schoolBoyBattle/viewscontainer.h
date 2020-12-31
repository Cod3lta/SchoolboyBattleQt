#ifndef VIEWSCONTAINER_H
#define VIEWSCONTAINER_H

#include <QMainWindow>
#include "view.h"
#include "game.h"

class ViewsContainer : public QMainWindow
{
    Q_OBJECT
public:
    ViewsContainer(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    Game *game;
    View *mainView;
signals:

};

#endif // VIEWSCONTAINER_H
