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

// Les textures sont étirées pour entrer dans le rectangle du joueur
#define PLAYER_WIDTH 100
#define PLAYER_HEIGHT 130


Player::Player(int id, int team, QGraphicsObject *parent)
    : QGraphicsObject(parent),
      id(id)
{
    // Spawn point des équipes
    teamsSpawnpoint.insert(red, {500, 500});
    teamsSpawnpoint.insert(black, {1000, 500});

    this->team = static_cast<Team>(team);
    gender = rand()%2 == 0 ? girl : boy;
    setPos(teamsSpawnpoint[this->team].at(0), teamsSpawnpoint[this->team].at(1));

    // L'animation dépends de gender et team :
    // Doit être après l'initialisation de ces variables !
    loadAnimations();
    setAnimation(idle);
    hitbox = QRectF(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT);
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

void Player::loadAnimations() {
    // L'animation doit se trouver dans les ressources sous le chemin
    // :/Resources/player/NAME/gender-team-NAME.png --> Exemples :
    // :/Resources/player/run/girl-red-run.png
    // :/Resources/player/idle/boy-black-idle.png
    animations.insert(idle, setupAnimation(500, 4, QString("idle")));
    animations.insert(run, setupAnimation(60, 10, QString("run")));
}

Player::AnimationsStruct* Player::setupAnimation(int framerate, int nbFrame, QString name) {
    AnimationsStruct* aStruct = new AnimationsStruct;
    aStruct->framerate = framerate;
    aStruct->nbFrame = nbFrame;
    aStruct->image = getAnimationByTeamAndGender(name);
    aStruct->timer = new QTimer();
    aStruct->timer->setInterval(aStruct->framerate);
    aStruct->timer->stop();
    connect(aStruct->timer, &QTimer::timeout, this, &Player::animationNextFrame);
    return aStruct;
}

void Player::setAnimation(Animation a) {
    // Arrêter le timer de l'animation qui se termine
    //animations.value(currentAnimation)->timer->stop();
    // Changer l'animation
    currentAnimation = a;
    // Démarer le timer de la nouvelle animation
    //animations.value(a)->timer->start();
}

QPixmap* Player::getAnimationByTeamAndGender(QString name) {
    QPixmap *p = new QPixmap();
    if(team == red) {
        if(gender == boy) {
            p->load(":/Resources/player/" + name + "/boy-red-" + name + ".png");
        }else if(gender == girl) {
            p->load(":/Resources/player/" + name + "/girl-red-" + name + ".png");
        }
    }else if(team == black) {
        if(gender == boy) {
            p->load(":/Resources/player/" + name + "/boy-black-" + name + ".png");
        }else if(gender == girl) {
            p->load(":/Resources/player/" + name + "/girl-black-" + name + ".png");
        }
    }
    return p;
}

void Player::animationNextFrame() {

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
    //QPixmap *imageToDraw = animations.value(currentAnimation)->image;
    //QRectF sourceRect = QRectF(pos().x(), pos().y(), imageToDraw->width(), imageToDraw->height());
    //QRectF targetRect = QRectF();

    //painter->drawPixmap(pos().x(), pos().y(), imageToDraw->width(), imageToDraw->height(), imageToDraw);
    //painter->drawPixmap(pos().x(), pos().y(), imageToDraw);

    // Debug rect
    painter->setPen(QPen(Qt::black));
    painter->drawRect(hitbox.x()+pos().x(), hitbox.y()+pos().y(), hitbox.width(), hitbox.height());
    painter->drawText(pos().x()+10, pos().y()+10, QString::number(id));
    // Lignes pour le compilateur
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

// Returns outer bounds of item as a rectangle
// Called by QGraphicsView to determine what regions need to be redrawn
QRectF Player::boundingRect() const {
    return QRectF(hitbox.x()+pos().x(), hitbox.y()+pos().y(), hitbox.width(), hitbox.height());
}

// collisions detection
QPainterPath Player::shape() const {
    QPainterPath path;
    path.addRect(hitbox.x()+pos().x(), hitbox.y()+pos().y(), hitbox.width(), hitbox.height());
    return path;
}


Player::~Player() {

}
