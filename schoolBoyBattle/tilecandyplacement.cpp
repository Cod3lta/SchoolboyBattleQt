#include "dataloader.h"
#include "tilecandyplacement.h"

#include <QPainter>
#include <QDebug>

#define TILE_SIZE 130
#define HITBOX_DEBUG true

TileCandyPlacement::TileCandyPlacement(
        int respawnDelayMs,
        int indexX,
        int indexY,
        DataLoader::TileLayerStruct* layerRessources,
        QString layer,
        int tileType,
        DataLoader *dataLoader,
        QGraphicsItem* parent) :
    Tile(indexX, indexY, layerRessources, layer, tileType, dataLoader, parent),
    respawnDelayMs(respawnDelayMs)
{
    int min = 1000, max = 10000;
    int randomDelayFirstSpawnMs = min + (rand() % static_cast<int>(max - min + 1));
    timer = new QTimer();
    timer->setInterval(respawnDelayMs + randomDelayFirstSpawnMs);
    timer->start();
    connect(timer, &QTimer::timeout, this, &TileCandyPlacement::spawnCandyTimer);
}

void TileCandyPlacement::spawnCandyTimer() {
    candySpawned = true;
    emit spawnCandy(
                x(),
                y(),
                dataLoader->getCandyRessources(tileType)->candyType,
                dataLoader->getCandyRessources(tileType)->candySize,
                this);
    timer->stop();
}


void TileCandyPlacement::candyPickedUp() {
    candySpawned = false;
    timer->setInterval(respawnDelayMs);
    timer->start();
}

// OVERRIDE REQUIRED

// Paints contents of item in local coordinates
void TileCandyPlacement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    if(HITBOX_DEBUG) {
        painter->setPen(QPen(Qt::blue));
        painter->drawRect(boundingRect());
        painter->drawText(10, 10, QString::number(timer->remainingTime()));
        painter->drawText(10, 30, QString::number(tileType));
        painter->setPen(QPen(Qt::red));
        painter->drawPath(shape());
    }

    // Lignes pour le compilateur
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

// Returns outer bounds of item as a rectangle
// Called by QGraphicsView to determine what regions need to be redrawn
// the rect stay at 0:0 !!
QRectF TileCandyPlacement::boundingRect() const {
    return QRectF(0, 0, TILE_SIZE, TILE_SIZE);
}

// collisions detection
QPainterPath TileCandyPlacement::shape() const {
    double widthRatio = 0.6;
    QPainterPath path;
    path.addRect(QRectF(
                     boundingRect().x() + (1 - widthRatio) * boundingRect().width() / 2,
                     boundingRect().y() + boundingRect().height() - boundingRect().height() * widthRatio,
                     boundingRect().width() * widthRatio,
                     boundingRect().height() * widthRatio * 2));
    return path;
}


TileCandyPlacement::~TileCandyPlacement() {

}
