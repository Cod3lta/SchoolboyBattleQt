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
    playersKeys.insert(Qt::Key_W,       {0, up});
    playersKeys.insert(Qt::Key_A,       {0, left});
    playersKeys.insert(Qt::Key_S,       {0, down});
    playersKeys.insert(Qt::Key_D,       {0, right});
    playersKeys.insert(Qt::Key_Up,      {1, up});
    playersKeys.insert(Qt::Key_Left,    {1, left});
    playersKeys.insert(Qt::Key_Down,    {1, down});
    playersKeys.insert(Qt::Key_Right,   {1, right});
}

// OVERRIDE REQUIRED

// Paints contents of item in local coordinates
void KeyInputs::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

// Returns outer bounds of item as a rectangle
// Called by QGraphicsView to determine what regions need to be redrawn
QRectF KeyInputs::boundingRect() const {
    return QRectF(0, 0, 0, 0);
}

// collisions detection
QPainterPath KeyInputs::shape() const {
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}


KeyInputs::~KeyInputs() {

}
