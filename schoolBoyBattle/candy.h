#ifndef CANDY_H
#define CANDY_H
#include <QGraphicsEllipseItem>

class Candy
{
public:
    Candy();

private:
    const static int nbCandies = 100;
    int pos_candy_x;
    int pos_candy_y;
    QGraphicsEllipseItem *candy;

    void randomItems();
};

#endif // CANDY_H
