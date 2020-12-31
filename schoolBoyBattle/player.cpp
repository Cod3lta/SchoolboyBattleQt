#include "player.h"
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QApplication>
#include <QKeyEvent>
#include <QRectF>
#include <QDebug>
#include <QVector2D>
#include "game.h"

Player::Player(int id, int x, int y, QGraphicsObject *parent)
    : QGraphicsObject(parent),
      id(id)
{
    setPos(x, y);
}

void Player::keyMove(int playerId, int direction, bool value) {
    if(playerId == id) {
        moves[direction] = value;
    }
}

void Player::move() {
    QVector2D v;
    v.setX(int(moves[right]) - int(moves[left]));
    v.setY(int(moves[down]) - int(moves[up]));
    v.normalize();
    setX(pos().x() + v.x());
    setY(pos().y() + v.y());
    update();
}

void Player::validate_candies() {

}

void Player::takeCandy() {

}



// OVERRIDE REQUIRED

// Paints contents of item in local coordinates
void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setPen(QPen(Qt::black));
    painter->setBrush(QBrush(Qt::red));
    painter->drawRect(pos().x(), pos().y(), 50, 50);
    painter->drawText(pos().x()+10, pos().y()+10, QString::number(id));
    // Lignes pour le compilateur
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

// Returns outer bounds of item as a rectangle
// Called by QGraphicsView to determine what regions need to be redrawn
QRectF Player::boundingRect() const {
    return QRectF(pos().x(), pos().y(), 100, 100);
}

// collisions detection
QPainterPath Player::shape() const {
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}


Player::~Player() {

}
