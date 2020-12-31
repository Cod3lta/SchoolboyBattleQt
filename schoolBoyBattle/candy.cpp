#include "candy.h"
#include <QBrush>
#include <QPen>
#include "game.h"

Candy::Candy()
{
    randomItems();
}

void Candy::randomItems()
{
    int pos_x, pos_y;
    QBrush yellowBrush(Qt::yellow);
    QPen outlinePen(Qt::black);
    outlinePen.setWidth(2);
    int max_x = 1800; //16777215;
    int max_y = 900;
    int min = -100;
    srand(time(NULL));

    for(int i=0; i<=nbCandies; i++)
    {
        pos_x = rand()%(max_x-min + 1) + min;
        pos_y = rand()%(max_y-min + 1) + min;
        //Game::scene->addEllipse(pos_x, pos_y, 20, 20, outlinePen, yellowBrush);
    }
}
