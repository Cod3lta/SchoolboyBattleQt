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
#define PLAYER_SPEED 5
#define HITBOX_DEBUG false



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
}

void Player::keyMove(int playerId, int direction, bool value) {
    if(playerId == id) {
        moves[direction] = value;
    }

    // Changer l'animation si nécessaire
    Animation newAnimationType = getAnimationType();
    if(currentAnimation != newAnimationType) {
        // Si l'animation qu'on doit afficher n'est pas la même que l'actuelle
        setAnimation(newAnimationType);
    }

    // Changer la direction du joueur si nécessaire
    facing = getFacing();
    update();
}

void Player::move(int delta) {
    QVector2D v;
    v.setX(int(moves[moveRight]) - int(moves[moveLeft]));
    v.setY(int(moves[moveDown]) - int(moves[moveUp]));
    v.normalize();
    double deltaMinified=delta/10e6;
    v*=deltaMinified*PLAYER_SPEED;
    setX(x() + v.x());
    setY(y() + v.y());
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
    animations.insert(idle, setupAnimation(150, 6, QString("idle")));
    animations.insert(run, setupAnimation(50, 10, QString("run")));
}

Player::AnimationsStruct* Player::setupAnimation(int framerate, int nbFrame, QString name) {
    AnimationsStruct* aStruct = new AnimationsStruct;
    aStruct->nbFrame = nbFrame;
    aStruct->frameIndex = 0;
    aStruct->image = getAnimationByTeamAndGender(name);
    aStruct->timer = new QTimer();
    aStruct->timer->setInterval(framerate);
    aStruct->timer->stop();
    connect(aStruct->timer, &QTimer::timeout, this, &Player::animationNextFrame);
    return aStruct;
}

void Player::setAnimation(Animation a) {
    // Arrêter le timer de l'animation qui se termine
    if(animations.contains(currentAnimation)) {
        animations.value(currentAnimation)->timer->stop();
    }
    // Changer l'animation
    currentAnimation = a;
    // Démarer le timer de la nouvelle animation
    animations.value(a)->timer->start();
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
    AnimationsStruct *a = animations.value(currentAnimation);
    a->frameIndex++;
    if(a->frameIndex >= a->nbFrame) {
        a->frameIndex = 0;
    }
    update();
}

Player::Animation Player::getAnimationType() {
    if((!moves[moveUp] && !moves[moveRight] && !moves[moveDown] && !moves[moveLeft]) ||
            (moves[moveUp] && moves[moveDown]) || (moves[moveRight] && moves[moveLeft])) {
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


    AnimationsStruct *animToDraw = animations.value(currentAnimation);
    QPixmap *imageToDraw = animToDraw->image;
    if(facing == facingLeft) {
        QTransform trans;
        trans.translate(boundingRect().width(), 0).scale(-1, 1);
        setTransform(trans);

    }else if (facing == facingRight) {
        setTransform(QTransform(1, 0, 0, 1, 1, 1));
    }else{
        resetTransform();
    }


    QRectF sourceRect = QRectF(imageToDraw->width() / animToDraw->nbFrame * animToDraw->frameIndex, 0,
                               imageToDraw->width() / animToDraw->nbFrame, imageToDraw->height());
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
    return QRectF(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT);
}

// collisions detection
QPainterPath Player::shape() const {
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}


Player::~Player() {

}
