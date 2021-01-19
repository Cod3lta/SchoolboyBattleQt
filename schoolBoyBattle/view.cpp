#include "view.h"
#include <QDebug>

#define LERP_AMOUNT 7

View::View(int id, QGraphicsView *parent)
    : QGraphicsView(parent),
      id(id)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFocusPolicy(Qt::NoFocus);    // Nécessaire pour ne pas pouvoir scroller
                                    // la scène avec la souris / les touches
                                    // flechées
}

void View::moveView(Player *player, int playerWidth, int playerHeight, double deltaMs) {
    x += ((player->x() + playerWidth/2) - x) / LERP_AMOUNT * deltaMs;
    y += ((player->y() + playerHeight/2) - y) / LERP_AMOUNT * deltaMs;
    centerOn(QPointF(x, y));
    //centerOn(player->pos());
    update();
}
