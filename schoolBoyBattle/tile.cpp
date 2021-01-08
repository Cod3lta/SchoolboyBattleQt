#include "dataloader.h"
#include "tile.h"
#include <QGraphicsItem>
#include <QPainter>

#define TILE_SIZE 130

Tile::Tile(int indexX, int indexY, int layer, int type, DataLoader *dataLoader, QGraphicsItem* parent)
    :QGraphicsItem(parent),
      indexX(indexX),
      indexY(indexY),
      type(type),
      layer(layer)
{
    image = dataLoader->getTileRessource(type);
    setPos(indexX * TILE_SIZE, indexY * TILE_SIZE);
}


// OVERRIDE REQUIRED

// Paints contents of item in local coordinates
void Tile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    painter->setPen(QPen(Qt::black));
    painter->setBrush(QBrush(Qt::white));
    //painter->drawRect(boundingRect());
    //painter->drawText(boundingRect().x()+10, boundingRect().y()+10, QString::number(type));

    QRectF sourceRect(0, 0, image->width(), image->height());
    painter->drawPixmap(boundingRect(), *image, sourceRect);

    // Lignes pour le compilateur
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

// Returns outer bounds of item as a rectangle
// Called by QGraphicsView to determine what regions need to be redrawn
// the rect stay at 0:0 !!
QRectF Tile::boundingRect() const {
    return QRectF(0, 0, TILE_SIZE, TILE_SIZE);
}

// collisions detection
QPainterPath Tile::shape() const {
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}


Tile::~Tile() {

}
