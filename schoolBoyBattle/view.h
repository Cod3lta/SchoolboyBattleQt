#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include "game.h"

class Game;

class View : public QGraphicsView
{
public:
    View(int id, QGraphicsView *parent = nullptr);

private:
    int id;
    Game *game;
};

#endif // VIEW_H
