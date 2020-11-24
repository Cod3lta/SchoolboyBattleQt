#include "player.h"
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QApplication>
#include <QKeyEvent>
#include <QRectF>
#include "game.h"

Player::Player(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    //scenePlayer = new QGraphicsScene(this);

    QBrush blueBrush(Qt::blue);
    QBrush redBrush(Qt::red);
    QPen outlinePen(Qt::black);
    outlinePen.setWidth(2);

    rectangle = Game::scene->addRect(1600, 0, 100, 100, outlinePen, blueBrush);
    ellipse = Game::scene->addEllipse(0, 800, 100, 100, outlinePen, redBrush);

    //QGraphicsView *view = new QGraphicsView(Game::scene, this);

    //QPixmap background(":/Resources/img/background/test.png");
    //Game::scene->setBackgroundBrush(background.scaled(1920, 1080,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));

    //view->resize(1920, 1080);

    //view->showFullScreen();

    //this->player_id = id_player;

    //view->show();
}


void Player::keyReleaseEvent(QKeyEvent *e)
{
    // Player1
        if(e->key() == Qt::Key_W) // haut
        {
            pos_player1_y = pos_player1_y-10;
            rectangle->setY(pos_player1_y);
        }
        if(e->key() == Qt::Key_S) // bas
        {
            pos_player1_y=pos_player1_y+10;
            rectangle->setY(pos_player1_y);
        }
        if(e->key() == Qt::Key_A) // gauche
        {
            pos_player1_x = pos_player1_x-10;
            rectangle->setX(pos_player1_x);
        }
        if(e->key() == Qt::Key_D) // droite
        {
            pos_player1_x = pos_player1_x+10;
            rectangle->setX(pos_player1_x);
        }


    // Player2
        if(e->key() == Qt::Key_Up) // haut
        {
            pos_player2_y = pos_player2_y-10;
            ellipse->setY(pos_player2_y);
        }
        if(e->key() == Qt::Key_Down) // bas
        {
            pos_player2_y = pos_player2_y+10;
            ellipse->setY(pos_player2_y);
        }
        if(e->key() == Qt::Key_Left) // gauche
        {
            pos_player2_x = pos_player2_x-10;
            ellipse->setX(pos_player2_x);
        }
        if(e->key() == Qt::Key_Right) // droite
        {
            pos_player2_x = pos_player2_x+10;
            ellipse->setX(pos_player2_x);
        }
}

void Player::validate_candies()
{

}

void Player::takeCandy()
{

}


// OVERRIDE REQUIRED
/*
// Paints contents of item in local coordinates
void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawRoundedRect(-10, -10, 20, 20, 5, 5);
}

// Returns outer bounds of item as a rectangle
// Called by QGraphicsView to determine what regions need to be redrawn
QRectF Player::boundingRect() const
{
    return QRectF(0, 0, 100, 100);
}

// collisions detection
QPainterPath Player::shape() const
{

    //QPainterPath ellipsePath;
    //ellipsePath.moveTo(80.0, 50.0);
    //ellipsePath.arcTo(20.0, 30.0, 60.0, 40.0, 0.0, 360.0);
    //return ellipsePath;


    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}
*/

Player::~Player()
{

}

// Afficher une image en background
// Comprendre (se documenter) sur les scènes + vues pour le splitscreen
