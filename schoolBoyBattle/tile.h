/*
 * Description : Chaque objet de cette classe est un carreau sur le terrain de jeu.
 *               Avec le support des couches de tiles, plusieurs tiles peuvent
 *               se trouver au même endroit.
 * Version     : 1.0.0
 * Date        : 25.01.2021
 * Auteurs     : Prétat Valentin, Badel Kevin et Margueron Yasmine
*/

#include "dataloader.h"
#include <QGraphicsObject>

#ifndef TILE_H
#define TILE_H

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
