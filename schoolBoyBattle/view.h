#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include "game.h"

class Game;

class View : public QGraphicsView
{
public:
    View(QGraphicsView *parent = nullptr);

private:
    Game *game;
};

#endif // VIEW_H
