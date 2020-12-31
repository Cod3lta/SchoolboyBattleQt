#include "view.h"
#include <QDebug>

View::View(QGraphicsView *parent)
    : QGraphicsView(parent)
{
    qDebug() << "view constructor called";
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFocusPolicy(Qt::NoFocus);    // Nécessaire pour ne pas pouvoir scroller
                                    // la scène avec la souris / les touches
                                    // flechées
}
