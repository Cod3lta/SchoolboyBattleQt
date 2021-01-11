#include "tilecandyplacement.h"

#include <QPainter>

#define TILE_SIZE 130
#define HITBOX_DEBUG true

TileCandyPlacement::TileCandyPlacement(
        int respawnDelayMs,
        int indexX,
        int indexY,
        int sceneTopLeftX,
        int sceneTopLeftY,
        QString layer,
        int type,
        DataLoader *dataLoader,
        QGraphicsItem* parent) :
    Tile(indexX, indexY, sceneTopLeftX, sceneTopLeftY, layer, type, dataLoader, parent),
    respawnDelayMs(respawnDelayMs)
{
    int min = 5000, max = 50;
    int delayFirstSpawnMs = min + (rand() % static_cast<int>(max - min + 1));
    timer = new QTimer();
    //QTimer *timer = new QTimer();
}

void TileCandyPlacement::takeCandy() {
    timer->setInterval(respawnDelayMs);
}

// OVERRIDE REQUIRED

// Paints contents of item in local coordinates
void TileCandyPlacement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    if(HITBOX_DEBUG) {
        painter->setPen(QPen(Qt::blue));
        painter->drawRect(boundingRect());
        painter->drawText(10, 10, QString::number(timer->remainingTime()));
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
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}


TileCandyPlacement::~TileCandyPlacement() {

}
