#include "candy.h"
#include <QBrush>
#include <QPen>
#include <QDebug>
#include "game.h"

#define CANDY_WIDTH 130
#define CANDY_HEIGHT 130
#define HITBOX_DEBUG false


Candy::Candy(int type, QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    this->type = static_cast<Type>(type);

}

// Setup des animations des candies ---------------------------------------------------------

/*QHash<Candy::Type, Candy::CandyAnimationsStruct*> Candy::loadCandyAnimationsDatas() {
    QHash<Candy::Type, Candy::CandyAnimationsStruct*> candiesData;
    candiesData.insert(peanut, setupCandyAnimationData(-1, 1, 1, "peanut"));
    candiesData.insert(mandarin, setupCandyAnimationData(-1, 1, 5, "mandarin"));
    return candiesData;
}

Candy::CandyAnimationsStruct* Candy::setupCandyAnimationData(int framerate, int nbFrame, int nbPoints, QString filename) {
    CandyAnimationsStruct *c = new CandyAnimationsStruct();
    c->frameIndex = 0;
    c->nbPoints = nbPoints;
    c->image = new QPixmap(":/Resources/candy/" + filename + ".png");
    c->nbFrame = nbFrame;
    // Si on donne un -1 pour le framerate, il n'y a pas d'animation
    if(framerate >= 0) {
        c->timer = new QTimer();
        c->timer->setInterval(framerate);
        c->timer->start();
    }
    qDebug() << "load animation " << filename << " for candies";
    return c;
}

//QHash<Candy::Type, Candy::CandyAnimationsStruct*> Candy::candiesAnimationsDatas =
*/

// Setup des placements des candies ---------------------------------------------------------



// OVERRIDE REQUIRED ------------------------------------------------------------------------

// Paints contents of item in local coordinates
void Candy::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    if(HITBOX_DEBUG) {
        // Debug rect
        painter->setPen(QPen(Qt::black));
        painter->setBrush(QBrush(Qt::white));
        painter->drawRect(boundingRect());
        painter->drawText(boundingRect().x()+10, boundingRect().y()+10, QString::number(id));
    }

    CandyAnimationsStruct *candyToDraw = candiesAnimations.value(type);
    QPixmap *imageToDraw = candyToDraw->image;

    QRectF sourceRect = QRectF(imageToDraw->width() / candyToDraw->nbFrame * candyToDraw->frameIndex, 0,
                               imageToDraw->width() / candyToDraw->nbFrame, imageToDraw->height());
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
