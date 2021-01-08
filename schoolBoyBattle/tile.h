#ifndef TILE_H
#define TILE_H

#include <QGraphicsItem>

#define TILE_SIZE 130

class Tile : public QGraphicsItem
{

public:
    Tile();

private:
    int i;
    int j;
    int layer;
};

#endif // TILE_H
