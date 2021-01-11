#include "dataloader.h"
#include "tile.h"
#include <QGraphicsItem>
#include <QPainter>

#define TILE_SIZE 130

Tile::Tile(int indexX, int indexY, DataLoader::TileLayerStruct* layerRessources, QString layer, int tileType, DataLoader *dataLoader, QGraphicsItem* parent)
    : QGraphicsObject(parent),
      tileType(tileType),
      layerRessources(layerRessources),
      layer(layer)
{
    image = dataLoader->getTileRessource(tileType)->image;
    int x = layerRessources->topLeftX * TILE_SIZE + indexX * TILE_SIZE;
    int y = layerRessources->topLeftY * TILE_SIZE + indexY * TILE_SIZE;
    setPos(x, y);
    setZValue(layerRessources->zIndex);
}


// OVERRIDE REQUIRED

// Paints contents of item in local coordinates
void Tile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {


    //painter->drawText(boundingRect().x()+10, boundingRect().y()+10, QString::number(type));

    if(!(layer == "4-collision" || layer == "5-config")) {
        QRectF sourceRect(0, 0, image->width(), image->height());
        painter->drawPixmap(boundingRect(), *image, sourceRect);
    }

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
