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
#define HITBOX_DEBUG false



Player::Player(
        int id,
        int team,
        DataLoader *dataLoader,
        int playerWidth, int playerHeight, int playerSpeed, 
        QGraphicsObject *parent)
    : QGraphicsObject(parent),
      dataLoader(dataLoader),
      id(id),
      playerWidth(playerWidth),
      playerHeight(playerHeight),
      playerSpeed(playerSpeed)
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
    setZIndex();
}

void Player::loadAnimations() {
    animations.insert(idle, setupAnimation(150, dataLoader->playerAnimations.value(dataLoader->getPlayerAnimationId(gender, team, idle))));
    animations.insert(run, setupAnimation(50, dataLoader->playerAnimations.value(dataLoader->getPlayerAnimationId(gender, team, run))));
}

Player::AnimationsLocalDatasStruct* Player::setupAnimation(int framerate, DataLoader::PlayerAnimationsStruct* sharedDatas) {
    AnimationsLocalDatasStruct* aStruct = new AnimationsLocalDatasStruct;
    aStruct->frameIndex = 0;
    aStruct->timer = new QTimer();
    aStruct->timer->setInterval(framerate);
    aStruct->timer->stop();
    connect(aStruct->timer, &QTimer::timeout, this, &Player::animationNextFrame);
    aStruct->sharedDatas = sharedDatas;
    return aStruct;
}

void Player::keyMove(int playerId, int direction, bool value) {
    if(playerId == id) {
        moves[direction] = value;
    }

    // Changer l'animation si nécessaire
    Animations newAnimationType = getAnimationType();
    if(currentAnimation != newAnimationType) {
        // Si l'animation qu'on doit afficher n'est pas la même que l'actuelle
        setAnimation(newAnimationType);
    }

    // Changer la direction du joueur si nécessaire
    facing = getFacing();
    update();
}

void Player::refresh(int delta) {
    move(delta);
    if(getAnimationType() == run) {
        setZIndex();
    }
}

void Player::setZIndex() {
    setZValue(y() + playerHeight);
}

void Player::move(int delta) {
    QVector2D v;
    v.setX(int(moves[moveRight]) - int(moves[moveLeft]));
    v.setY(int(moves[moveDown]) - int(moves[moveUp]));
    v.normalize();
    double deltaMinified=delta/10e6;
    v*=deltaMinified * playerSpeed;
    setX(x() + v.x());
    setY(y() + v.y());
}

void Player::validate_candies() {

}

void Player::takeCandy() {

}

void Player::setAnimation(Animations a) {
    // Arrêter le timer de l'animation qui se termine
    if(animations.contains(currentAnimation)) {
        animations.value(currentAnimation)->timer->stop();
    }
    // Changer l'animation
    currentAnimation = a;
    // Démarer le timer de la nouvelle animation
    animations.value(a)->timer->start();
}

void Player::animationNextFrame() {
    AnimationsLocalDatasStruct *a = animations.value(currentAnimation);
    a->frameIndex++;
    if(a->frameIndex >= a->sharedDatas->nbFrame) {
        a->frameIndex = 0;
    }
    update();
}

Player::Animations Player::getAnimationType() {
    if((!moves[moveUp] && !moves[moveRight] && !moves[moveDown] && !moves[moveLeft]) ||
            (moves[moveUp] && moves[moveDown] && !moves[moveLeft] && !moves[moveRight]) ||
            (moves[moveRight] && moves[moveLeft] && !moves[moveUp] && !moves[moveDown]) ||
            (moves[moveRight] && moves[moveLeft] && moves[moveUp] && moves[moveDown])){
        return idle;
    }
    return run;
}

Player::Facing Player::getFacing() {
    if(moves[moveLeft] && !moves[moveRight]) {
        return facingLeft;
    }else if(!moves[moveLeft] && moves[moveRight]) {
        return facingRight;
    }
    return facing;
}

// OVERRIDE REQUIRED

// Paints contents of item in local coordinates
void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {


    if(HITBOX_DEBUG) {
        // Debug rect
        painter->setPen(QPen(Qt::black));
        painter->setBrush(QBrush(Qt::white));
        painter->drawRect(boundingRect());
        painter->drawText(boundingRect().x()+10, boundingRect().y()+10, QString::number(id));
    }


    AnimationsLocalDatasStruct *animToDraw = animations.value(currentAnimation);
    QPixmap *imageToDraw = animToDraw->sharedDatas->image;
    if(facing == facingLeft) {
        QTransform trans;
        trans.translate(boundingRect().width(), 0).scale(-1, 1);
        setTransform(trans);

    }else if (facing == facingRight) {
        setTransform(QTransform(1, 0, 0, 1, 1, 1));
    }else{
        resetTransform();
    }


    QRectF sourceRect = QRectF(imageToDraw->width() / animToDraw->sharedDatas->nbFrame * animToDraw->frameIndex, 0,
                               imageToDraw->width() / animToDraw->sharedDatas->nbFrame, imageToDraw->height());
    QRectF targetRect = boundingRect();
    painter->drawPixmap(targetRect, *imageToDraw, sourceRect);

    // Lignes pour le compilateur
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

// Returns outer bounds of item as a rectangle
// Called by QGraphicsView to determine what regions need to be redrawn
// the rect stay at 0:0 !!
QRectF Player::boundingRect() const {
    return QRectF(0, 0, playerWidth, playerHeight);
}

// collisions detection
QPainterPath Player::shape() const {
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}


Player::~Player() {

}
