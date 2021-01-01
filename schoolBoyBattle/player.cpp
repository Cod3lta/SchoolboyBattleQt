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

Player::Player(int id, int team, QGraphicsObject *parent)
    : QGraphicsObject(parent),
      id(id)
{
    // Spawn point des équipes
    teamsSpawnpoint.insert(red, {500, 500});
    teamsSpawnpoint.insert(black, {1000, 500});

    this->team = static_cast<Teams>(team);
    gender = rand()%2 == 0 ? girl : boy;
    setPos(teamsSpawnpoint[this->team].at(0), teamsSpawnpoint[this->team].at(1));
    //animationIdle->load(":/Resources/player/idle.png");

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
    switch(gender) {
    case boy:
        painter->setBrush(QBrush(Qt::red));
        break;
    case girl:
        painter->setBrush(QBrush(Qt::yellow));
        break;
    }

    painter->setPen(QPen(Qt::black));
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
