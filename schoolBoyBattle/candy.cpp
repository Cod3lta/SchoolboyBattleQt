#include "candy.h"
#include <QBrush>
#include <QPen>
#include <QDebug>
#include "game.h"

#define CANDY_WIDTH 130
#define CANDY_HEIGHT 130
#define HITBOX_DEBUG false
#define LERP_AMOUNT 50 // plus haut = distance parcourue plus petite
#define LERP_ACCELERATION 40


Candy::Candy(
        int x,
        int y,
        int candyType,
        int candySize,
        DataLoader *dataLoader,
        TileCandyPlacement *tilePlacement,
        QGraphicsObject *parent)
    :
      QGraphicsObject(parent),
      idTeam(-1),
      candyType(static_cast<Type>(candyType)),
      candySize(static_cast<Size>(candySize)),
      dataLoader(dataLoader),
      tilePlacement(tilePlacement),
      taken(false)
{
    loadAnimations();
    setAnimation(idle);
    setPos(x, y);
    setZIndex();
    if(tilePlacement != nullptr) {
        connect(this, &Candy::pickedUp, tilePlacement, &TileCandyPlacement::candyPickedUp);
    }
}

// Setup des animations des candies ---------------------------------------------------------

void Candy::loadAnimations() {
    animationsLocal.insert(
                idle,
                setupCandyAnimationData(
                    dataLoader->candyAnimations.value(
                        dataLoader->getCandyAnimationId(candyType, candySize))));
}

Candy::AnimationsLocalStruct* Candy::setupCandyAnimationData(DataLoader::CandyAnimationsStruct* sharedDatas) {
    AnimationsLocalStruct *c = new AnimationsLocalStruct();
    c->frameIndex = 0;
    c->sharedDatas = sharedDatas;
    // Si on donne un -1 pour le framerate, il n'y a pas d'animation
    c->timer = new QTimer();
    if(sharedDatas->framerate >= 0) {
        c->timer->setInterval(sharedDatas->framerate);
        c->timer->stop();
    }
    connect(c->timer, &QTimer::timeout, this, &Candy::animationNextFrame);
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

void Candy::pickUp(QGraphicsItem *player, int team) {
    taken = true;
    emit pickedUp();
    currentPlayer = player;
    idTeam = team;
}

bool Candy::isTaken() {
    return taken;
}

void Candy::setCurrentPlayer(QGraphicsItem *player, int team) {
    currentPlayer = player;
    idTeam = team;
}

QGraphicsItem *Candy::getCurrentPlayer() {
    return currentPlayer;
}

void Candy::refresh(QPointF pos, int posInQueue, double delta) {
    if(!taken) return;
    int yOffset = 0;
    int lerpFactor = (LERP_AMOUNT * LERP_ACCELERATION) / (LERP_AMOUNT + posInQueue);
    if(posInQueue == 0) yOffset = dataLoader->getPlayerSize().y() / 8;
    setX(this->x() + (pos.x() - this->x()          ) / lerpFactor * delta);
    setY(this->y() + (pos.y() - this->y() + yOffset) / lerpFactor * delta);

    setZIndex();
}

// OVERRIDE REQUIRED ------------------------------------------------------------------------

// Paints contents of item in local coordinates
void Candy::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    if(HITBOX_DEBUG) {
        // Debug rect
        painter->setPen(QPen(Qt::yellow));
        painter->drawRect(boundingRect());
        painter->setPen(QPen(Qt::red));
        painter->drawPath(shape());
        painter->drawText(10, 10, QString::number(id));
    }

    AnimationsLocalStruct *candyToDraw = animationsLocal.value(animation);
    QPixmap *imageToDraw = candyToDraw->sharedDatas->image;
    QPixmap *imageHover = candyToDraw->sharedDatas->image;
    if(idTeam == 0)
         imageHover = candyToDraw->sharedDatas->imageRed;
    else if(idTeam == 1)
         imageHover = candyToDraw->sharedDatas->imageBlack;

    QRectF sourceRect = QRectF(imageToDraw->width() / candyToDraw->sharedDatas->nbFrame * candyToDraw->frameIndex, 0,
                               imageToDraw->width() / candyToDraw->sharedDatas->nbFrame, imageToDraw->height());
    QRectF targetRect = boundingRect();

    painter->drawPixmap(targetRect, *imageToDraw, sourceRect);
    if(idTeam != -1) painter->drawPixmap(targetRect, *imageHover, sourceRect);

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
    path.addRect(QRectF(
                     boundingRect().x() + boundingRect().width() / 4,
                     boundingRect().y() + boundingRect().height() / 4,
                     boundingRect().width()/2,
                     boundingRect().height()/2));
    return path;
}


Candy::~Candy() {

}
