#include "keyinputs.h"
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QApplication>
#include <QKeyEvent>
#include <QRectF>
#include <QDebug>

KeyInputs::KeyInputs(QGraphicsObject *parent)
    : QGraphicsObject(parent)
{
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setPlayerKeys();
}

void KeyInputs::keyPress(QKeyEvent * event) {
    int key = event->key();
    if(playersKeys.contains(key)) {
        int playerId = playersKeys.value(key).at(0);
        int playerMove = playersKeys.value(key).at(1);
        emit playerKeyToggle(
                    playerId,
                    playerMove,
                    true
        );
    }
}

void KeyInputs::keyRelease(QKeyEvent *event) {
    int key = event->key();
    if(playersKeys.contains(key)) {
        int playerId = playersKeys.value(key).at(0);
        int playerMove = playersKeys.value(key).at(1);
        emit playerKeyToggle(
                    playerId,
                    playerMove,
                    false
        );
    }
}

void KeyInputs::setPlayerKeys() {
    // La clé à presser, {l'id du joueur, la direction}
    playersKeys.insert(Qt::Key_W,       {1, up});
    playersKeys.insert(Qt::Key_A,       {1, left});
    playersKeys.insert(Qt::Key_S,       {1, down});
    playersKeys.insert(Qt::Key_D,       {1, right});
    playersKeys.insert(Qt::Key_Up,      {2, up});
    playersKeys.insert(Qt::Key_Left,    {2, left});
    playersKeys.insert(Qt::Key_Down,    {2, down});
    playersKeys.insert(Qt::Key_Right,   {2, right});
}

// OVERRIDE REQUIRED

// Paints contents of item in local coordinates
void KeyInputs::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawRoundedRect(-10, -10, 20, 20, 5, 5);
}

// Returns outer bounds of item as a rectangle
// Called by QGraphicsView to determine what regions need to be redrawn
QRectF KeyInputs::boundingRect() const
{
    return QRectF(0, 0, 100, 100);
}

// collisions detection
QPainterPath KeyInputs::shape() const
{

    //QPainterPath ellipsePath;
    //ellipsePath.moveTo(80.0, 50.0);
    //ellipsePath.arcTo(20.0, 30.0, 60.0, 40.0, 0.0, 360.0);
    //return ellipsePath;


    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}


KeyInputs::~KeyInputs()
{

}
