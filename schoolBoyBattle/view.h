#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include "game.h"

class Game;

class View : public QGraphicsView
{
    Q_OBJECT
public:
    View(int id, QGraphicsView *parent = nullptr);

    void moveView(Player *player);
private:
    int id;
    Game *game;
};

#endif // VIEW_H
