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
            int sceneTopLeftX,
            int sceneTopLeftY,
            QString layer,
            int type,                   // numéro qu'il y a dans le .tmx
            DataLoader *dataLoader,
            QGraphicsItem* parent = nullptr);
    ~Tile();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    int tileType;

private:
    DataLoader *dataLoader;
    QString layer;
    QPixmap* image;
};

#endif // TILE_H
