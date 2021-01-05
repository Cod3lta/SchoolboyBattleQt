#include "view.h"
#include <QDebug>

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

void View::moveView(Player *player) {
    centerOn(player);
    update();
}
