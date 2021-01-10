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
        int tileType,
        DataLoader *dataLoader,
        QGraphicsItem* parent) :
    Tile(indexX, indexY, sceneTopLeftX, sceneTopLeftY, layer, tileType, dataLoader, parent),
    respawnDelayMs(respawnDelayMs)
{
    int min = 5000, max = 10000;
    int randomDelayFirstSpawnMs = min + (rand() % static_cast<int>(max - min + 1));
    timer = new QTimer();
    timer->setInterval(respawnDelayMs + randomDelayFirstSpawnMs);
    timer->start();
    connect(timer, &QTimer::timeout, this, &TileCandyPlacement::spawnCandy);
}

void TileCandyPlacement::spawnCandy() {
    candySpawned = true;
}


void TileCandyPlacement::takeCandy() {
    candySpawned = false;
    timer->setInterval(respawnDelayMs);
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
