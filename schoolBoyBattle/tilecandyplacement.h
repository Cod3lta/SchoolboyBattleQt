#ifndef TILECANDYPLACEMENT_H
#define TILECANDYPLACEMENT_H

#include "tile.h"

#include <QGraphicsObject>

class TileCandyPlacement : public Tile
{
public:
    TileCandyPlacement(
            int respawnDelayMs,
            int indexX,                         // Données nécessaires pour créer un objet Tile
            int indexY,
            DataLoader::TileLayerStruct* layerRessources,
            QString layer,
            int type,
            DataLoader *dataLoader,
            QGraphicsItem* parent = nullptr);
    ~TileCandyPlacement();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    bool taken;
    int respawnDelayMs;
    QTimer *timer;

public slots:
    void takeCandy();

};

#endif // TILECANDYPLACEMENT_H
