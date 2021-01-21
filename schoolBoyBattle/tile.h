#ifndef TILE_H
#define TILE_H

#include "dataloader.h"

#include <QGraphicsObject>


class Tile : public QGraphicsObject
{
    Q_OBJECT

public:
    Tile(
            int x,
            int y,
            DataLoader::TileLayerStruct* layerRessources,
            QString layer,
            int tileType,                   // numéro qu'il y a dans le .tmx
            DataLoader *dataLoader,
            QGraphicsItem* parent = nullptr);
    ~Tile();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    int getTileType();
    DataLoader::TileLayerStruct* layerRessources;

private:
    QString layer;
    QPixmap* image;
    QRectF boundingRectangle;

protected:
    // Pour les tiles de type TileCandyPlacement
    int tileType;
    DataLoader *dataLoader;

};

#endif // TILE_H
