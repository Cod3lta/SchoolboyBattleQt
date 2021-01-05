#include "game.h"
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QSet>

#include "candy.h"
#include "player.h"
#include "keyinputs.h"

Game::Game(QGraphicsScene *parent)
    : QGraphicsScene(parent)
{

//    QBrush blueBrush(Qt::blue);
//    QPen outlinePen(Qt::black);
//    outlinePen.setWidth(2);
//    scene->addEllipse(0, 0, 50, 50, outlinePen, blueBrush);

    QPixmap background(":/Resources/img/background/terrain1.png");
    setBackgroundBrush(background);
    setSceneRect(background.rect());
    //setSceneRect(0, 0, 100, 100);

    timer = new QTimer(this);
    timer->setInterval(15);
    connect(timer, &QTimer::timeout, this, &Game::playerMove);
    timer->start();

    // TODO : Afficher les bonbons sur le terrain


    //player1 = new Player(1, sceneRect().width()/2 + 50, sceneRect().height()/2);
    //player2 = new Player(2, sceneRect().width()/2 - 50, sceneRect().height()/2);
    player1 = new Player(1, 550, 500);
    player2 = new Player(2, 450, 500);
    keyboardInputs = new KeyInputs();

    // Connecter les signaux de keyboardInputs aux slots des joueurs pour le clavier
    connect(keyboardInputs, &KeyInputs::playerKeyToggle, player1, &Player::keyMove);
    connect(keyboardInputs, &KeyInputs::playerKeyToggle, player2, &Player::keyMove);

    addItem(player1);
    addItem(player2);
    addItem(keyboardInputs);
}

void Game::keyPress(QKeyEvent *event) {
    keyboardInputs->keyPress(event);
}

void Game::keyRelease(QKeyEvent *event) {
    keyboardInputs->keyRelease(event);
}


void Game::playerMove() {
    player1->move();
    player2->move();
}

void Game::reset()
{

}
