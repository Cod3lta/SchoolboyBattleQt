#include "view.h"

View::View(QGraphicsView *parent)
    : QGraphicsView(parent)
{
    game = new Game();

    setScene(game->scene);
}
