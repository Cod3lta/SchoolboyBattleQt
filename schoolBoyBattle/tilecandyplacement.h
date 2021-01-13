#ifndef TILECANDYPLACEMENT_H
#define TILECANDYPLACEMENT_H

#include "tile.h"

#include <QGraphicsObject>

class TileCandyPlacement : public Tile
{
    Q_OBJECT
public:
    TileCandyPlacement(
            int respawnDelayMs,
            int indexX,                         // Données nécessaires pour créer un objet Tile
            int indexY,
            DataLoader::TileLayerStruct* layerRessources,
            QString layer,
            int tileType,
            DataLoader *dataLoader,
            QGraphicsItem* parent = nullptr);
    ~TileCandyPlacement();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    bool candySpawned;
    int respawnDelayMs;
    QTimer *timer;

public slots:
    void spawnCandyTimer();
    void candyPickedUp();

signals:
    void spawnCandy(int x, int y, int candyType, int candySize, TileCandyPlacement* tilePlacement);

};

#endif // TILECANDYPLACEMENT_H
