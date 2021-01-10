#ifndef TILE_H
#define TILE_H

#include "dataloader.h"

#include <QGraphicsItem>


class Tile : public QGraphicsItem
{

public:
    Tile(
            int x,
            int y,
            DataLoader::TileLayerStruct* layerRessources,
            QString layer,
            int type,
            DataLoader *dataLoader,
            QGraphicsItem* parent = nullptr);
    ~Tile();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    int type;
    DataLoader::TileLayerStruct* layerRessources;

private:
    QString layer;
    DataLoader *dataLoader;
    QPixmap* image;
};

#endif // TILE_H
