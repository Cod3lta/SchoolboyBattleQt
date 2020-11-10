#include "player.h"
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QApplication>

Player::Player(QWidget *parent)
    : QMainWindow(parent)
{
    scenePlayer = new QGraphicsScene(this);

    QBrush blueBrush(Qt::blue);
    QBrush redBrush(Qt::red);
    QPen outlinePen(Qt::black);
    outlinePen.setWidth(2);

    scenePlayer->addEllipse(0, 0, 50, 50, outlinePen, blueBrush);
    scenePlayer->addEllipse(0, 0, 50, 50, outlinePen, blueBrush);

    QGraphicsView *view = new QGraphicsView(scenePlayer, this);

    QPixmap background(":/Resources/img/background/test.png");
    scenePlayer->setBackgroundBrush(background.scaled(1920, 1080,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));

    view->resize(1920, 1080);


    view->showFullScreen();

    //view->show();
}


void Player::move(int x, int y)
{

}
void Player::validate_candies()
{

}
void Player::takeCandy()
{

}


Player::~Player()
{
}

// Afficher une image en background
// Comprendre (se documenter) sur les scènes + vues pour le splitscreen
