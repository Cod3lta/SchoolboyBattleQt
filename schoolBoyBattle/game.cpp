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

Game::Game(int nbPlayers, QGraphicsScene *parent)
    : QGraphicsScene(parent)
{
    QPixmap background(":/Resources/img/background/terrain1.png");
    setBackgroundBrush(background);
    setSceneRect(background.rect());
    //setSceneRect(0, 0, 100, 100);

    timer = new QTimer(this);
    timer->setInterval(5);
    connect(timer, &QTimer::timeout, this, &Game::playerMoveTimer);
    timer->start();

    // Afficher les bonbons sur le terrain
    Candy();

    // Joueurs
    for(int i = 0; i < nbPlayers; i++) {
        players.append(new Player(i, 500, 500));
    }

    keyboardInputs = new KeyInputs();

    // Connecter les signaux de keyboardInputs aux slots des joueurs pour le clavier
    for (int i = 0; i < players.size(); ++i) {
        connect(keyboardInputs, &KeyInputs::playerKeyToggle, players.at(i), &Player::keyMove);
    }

    // Ajouter les items sur la scène
    for (int i = 0; i < players.size(); ++i) {
        addItem(players.at(i));
    }
    addItem(keyboardInputs);
}

void Game::keyPress(QKeyEvent *event) {
    keyboardInputs->keyPress(event);
}

void Game::keyRelease(QKeyEvent *event) {
    keyboardInputs->keyRelease(event);
}


void Game::playerMoveTimer() {
    for (int i = 0; i < players.size(); ++i) {
        players.at(i)->move();

    }
}

void Game::reset()
{

}
