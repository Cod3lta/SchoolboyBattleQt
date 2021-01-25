#ifndef TILECANDYPLACEMENT_H
#define TILECANDYPLACEMENT_H

#include "tile.h"

#include <QGraphicsObject>

class TileCandyPlacement : public Tile
{
    Q_OBJECT
public:
    TileCandyPlacement(
            int id,                             // Utilisé quand une autre instance broadcast de
                                                // créer un candy à un certain emplacement
            int respawnDelayMs,
            int indexX,                         // Données nécessaires pour créer un objet Tile
            int indexY,
            DataLoader::TileLayerStruct* layerRessources,
            QString layer,
            int tileType,
            DataLoader *dataLoader,
            QGraphicsItem* parent = nullptr);
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    bool candySpawned;
    int respawnDelayMs;
    int id;
    int min, max;
    static int candyId;
    QTimer *timer;

public slots:
    void spawnCandyTimer();
    void candyPickedUp();

signals:
    void spawnCandy(int candyType, int candySize, int nbPoints, int tilePlacementId, int candyId);

};

#endif // TILECANDYPLACEMENT_H
