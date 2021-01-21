#include "dataloader.h"
#include "tile.h"
#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>

#define IMAGE_PIXEL_SIZE 13

Tile::Tile(int indexX, int indexY, DataLoader::TileLayerStruct* layerRessources, QString layer, int tileType, DataLoader *dataLoader, QGraphicsItem* parent)
    :QGraphicsObject(parent),
      layer(layer),
      tileType(tileType),
    dataLoader(dataLoader)
{
    int tileSize = dataLoader->getTileSize();
    image = dataLoader->getTileRessource(tileType)->image;
    int x = layerRessources->topLeftX * tileSize + indexX * tileSize;
    int y = layerRessources->topLeftY * tileSize + indexY * tileSize;
    int ratio = tileSize / IMAGE_PIXEL_SIZE;
    int tileWidth = image->width() * ratio;
    int tileHeight = image->height() * ratio;

    prepareGeometryChange();
    boundingRectangle = QRectF(0, tileSize - tileHeight, tileWidth, tileHeight);

    setPos(x, y);

    if(layerRessources->zIndex.userType() == QMetaType::Int)
        setZValue(layerRessources->zIndex.toInt());
    else
        setZValue(y + tileSize);

}


// OVERRIDE REQUIRED

// Paints contents of item in local coordinates
void Tile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {


    //painter->drawText(boundingRect().x()+10, boundingRect().y()+10, QString::number(type));

    if(!(layer == "1-spawns" || layer == "4-collision" || layer == "5-config")) {
        QRectF sourceRect(0, 0, image->width(), image->height());
        painter->drawPixmap(boundingRect(), *image, sourceRect);
    }

    // Lignes pour le compilateur
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

int Tile::getTileType() {
    return tileType;
}

// Returns outer bounds of item as a rectangle
// Called by QGraphicsView to determine what regions need to be redrawn
// the rect stay at 0:0 !!
QRectF Tile::boundingRect() const {
    return boundingRectangle;
}

// collisions detection
QPainterPath Tile::shape() const {
    QPainterPath path;
    path.addRect(QRect(0, 0, dataLoader->getTileSize(), dataLoader->getTileSize()));
    return path;
}


Tile::~Tile() {

}
