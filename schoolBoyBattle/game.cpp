#include "game.h"
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QApplication>

#include "candy.h"
#include "player.h"

QGraphicsScene* Game::scene = NULL;

Game::Game(QGraphicsScene *parent)
    : QGraphicsScene(parent)
{
    scene = new QGraphicsScene(this);

    QBrush blueBrush(Qt::blue);
    QPen outlinePen(Qt::black);
    outlinePen.setWidth(2);
    //scene->addEllipse(0, 0, 50, 50, outlinePen, blueBrush);

    //QGraphicsView *view = new QGraphicsView(scene, this);

    QPixmap background(":/Resources/img/background/test.png");
    scene->setBackgroundBrush(background.scaled(1920, 1080,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));

//    scene->setSceneRect(scene->itemsBoundingRect());
//    view->setSceneRect(view->sceneRect());
//    view->showFullScreen();
//    view->setWindowState(Qt::WindowFullScreen);
    //TODO: Remplacer ce truc un peu moche
    //view->resize(1920, 1080);

    //view->setWindowState(Qt::WindowMaximized);
    //view->showFullScreen();

    //view->show();

    // Afficher les bonbons sur le terrain
    Candy();

    // Ajouter les joueurs
    /*
    player1 = new Player();
    player2 = new Player();
    addItem(player1);
    addItem(player2);
    */

    player1 = new Player();




}

//// pas utile, vu que la scene est en static mtn... donc accessible
//void Game::addItemInMap(QGraphicsItem* some)
//{
//    scene->addItem(some);
//}


void Game::timer()
{

}

void Game::reset()
{

}


Game::~Game()
{
}

// Afficher une image en background
// Comprendre (se documenter) sur les scènes + vues pour le splitscreen
