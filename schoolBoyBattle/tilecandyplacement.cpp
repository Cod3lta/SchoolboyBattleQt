/*
 * Description : Cette classe dérive de Tile et a pour but de faire
 *               apparaître des bonbons à intervalles réguliers sur le terrain.
 * Version     : 1.0.0
 * Date        : 25.01.2021
 * Auteurs     : Prétat Valentin, Badel Kevin et Margueron Yasmine
*/

#include "dataloader.h"
#include "tilecandyplacement.h"

#include <QPainter>
#include <QDebug>

#define TILE_SIZE 130
#define HITBOX_DEBUG false

int TileCandyPlacement::candyId = 0;

TileCandyPlacement::TileCandyPlacement(
        int id,
        int respawnDelayMs,
        int indexX,
        int indexY,
        DataLoader::TileLayerStruct* layerRessources,
        QString layer,
        int tileType,
        DataLoader *dataLoader,
        QGraphicsItem* parent) :
    Tile(indexX, indexY, layerRessources, layer, tileType, dataLoader, parent),
    respawnDelayMs(respawnDelayMs),
    id(id),
    min(0),
    max(10 * 1000)
{
    int randomDelayFirstSpawnMs = min + (rand() % static_cast<int>(max - min + 1));
    timer = new QTimer();
    timer->setInterval(randomDelayFirstSpawnMs);
    timer->start();
    connect(timer, &QTimer::timeout, this, &TileCandyPlacement::spawnCandyTimer);
}

void TileCandyPlacement::spawnCandyTimer() {
    // Si le spawner ne se trouve dans la vue d'aucun joueur (peut-être todo)
    candySpawned = true;
    emit spawnCandy(
                dataLoader->getCandyRessources(tileType)->candyType,
                dataLoader->getCandyRessources(tileType)->candySize,
                dataLoader->getCandyRessources(tileType)->nbPoints,
                this->id,
                candyId);
    candyId++;
    timer->stop();
}


void TileCandyPlacement::candyPickedUp() {
    candySpawned = false;
    int randomDelay = min + (rand() % static_cast<int>(max - min + 1));
    timer->setInterval(respawnDelayMs + randomDelay);
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
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}
