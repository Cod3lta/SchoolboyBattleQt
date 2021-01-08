#ifndef TILE_H
#define TILE_H

#include <QGraphicsItem>


class Tile : public QGraphicsItem
{

public:
    Tile(int x, int y, int layer, int type, QGraphicsItem* parent = nullptr);
    ~Tile();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    int indexX;
    int indexY;
    int type;
    int layer;
    QPixmap* image;
};

#endif // TILE_H
