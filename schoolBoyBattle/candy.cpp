#include "candy.h"
#include <QBrush>
#include <QPen>
#include <QDebug>
#include "game.h"

#define CANDY_WIDTH 130
#define CANDY_HEIGHT 130
#define HITBOX_DEBUG false


Candy::Candy(
        int x,
        int y,
        int candyType,
        int candySize,
        DataLoader *dataLoader,
        QGraphicsItem *parent)
    : QGraphicsItem(parent),
      candyType(static_cast<Type>(candyType)),
      candySize(static_cast<Size>(candySize)),
      dataLoader(dataLoader)
{
    loadAnimations();
    setAnimation(idle);
    setPos(x, y);
    setZIndex();
}

// Setup des animations des candies ---------------------------------------------------------

void Candy::loadAnimations() {
    animationsLocal.insert(
                idle,
                setupCandyAnimationData(
                    -1,
                    dataLoader->candyAnimations.value(
                        dataLoader->getCandyAnimationId(candyType, candySize))));
}

Candy::AnimationsLocalStruct* Candy::setupCandyAnimationData(int framerate, DataLoader::CandyAnimationsStruct* sharedDatas) {
    AnimationsLocalStruct *c = new AnimationsLocalStruct();
    c->frameIndex = 0;
    c->sharedDatas = sharedDatas;
    // Si on donne un -1 pour le framerate, il n'y a pas d'animation
    c->timer = new QTimer();
    if(framerate >= 0) {
        c->timer->setInterval(framerate);
        c->timer->stop();
    }
    return c;
}

// Setup des placements des candies ---------------------------------------------------------


// autres trucs de la classe ----------------------------------------------------------------

void Candy::animationNextFrame() {
    AnimationsLocalStruct *a = animationsLocal.value(animation);
    a->frameIndex++;
    if(a->frameIndex >= a->sharedDatas->nbFrame) {
        a->frameIndex = 0;
    }
    update();
}

void Candy::setAnimation(Animations a) {
    // Arrêter le timer de l'animation qui se termine
    if(animationsLocal.contains(animation)) {
        animationsLocal.value(animation)->timer->stop();
    }
    // Changer l'animation
    animation = a;
    // Démarer le timer de la nouvelle animation
    animationsLocal.value(a)->timer->start();
}

void Candy::setZIndex() {
    setZValue(y() + CANDY_HEIGHT * 0.8);
}


// OVERRIDE REQUIRED ------------------------------------------------------------------------

// Paints contents of item in local coordinates
void Candy::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    if(HITBOX_DEBUG) {
        // Debug rect
        painter->setPen(QPen(Qt::yellow));
        painter->drawRect(boundingRect());
        painter->drawText(10, 10, QString::number(id));
    }

    AnimationsLocalStruct *candyToDraw = animationsLocal.value(animation);
    QPixmap *imageToDraw = candyToDraw->sharedDatas->image;

    QRectF sourceRect = QRectF(imageToDraw->width() / candyToDraw->sharedDatas->nbFrame * candyToDraw->frameIndex, 0,
                               imageToDraw->width() / candyToDraw->sharedDatas->nbFrame, imageToDraw->height());
    QRectF targetRect = boundingRect();
    painter->drawPixmap(targetRect, *imageToDraw, sourceRect);

    // Lignes pour le compilateur
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

// Returns outer bounds of item as a rectangle
// Called by QGraphicsView to determine what regions need to be redrawn
// the rect stay at 0:0 !!
QRectF Candy::boundingRect() const {
    return QRectF(0, 0, CANDY_WIDTH, CANDY_HEIGHT);
}

// collisions detection
QPainterPath Candy::shape() const {
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}


Candy::~Candy() {

}
