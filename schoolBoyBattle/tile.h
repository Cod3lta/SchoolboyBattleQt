#ifndef TILE_H
#define TILE_H

#include <QGraphicsItem>

class Tile : public QGraphicsItem
{
public:
    Tile();
    enum Type {mur, sol, facade};

private:
    QPixmap img;
    Type type;
};

#endif // TILE_H
