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
#include "view.h"
#include "menu.h"
#include "tile.h"

Game::Game(int nbPlayers, QGraphicsScene *parent)
    : QGraphicsScene(parent)
{

    //menu
    //menuJeu = new Menu();
    //menuJeu->afficherMenuPrincipal();


    //scene du niveau

    //    QPixmap background(":/Resources/background/terrain.png");
    //    setBackgroundBrush(background);
    //    setSceneRect(background.rect());
    //    //setSceneRect(0, 0, 100, 100);

    Tile *tile = new Tile(Tile::Type::sol, 0, 0, 0);
    QFile *filename = new QFile("terrain1.tmx");
    tile->prepareScene(filename);
    //Tile tile(":/Resources/background/terrain1.tmx");


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
        Player *player = players.at(i);
        player->move();
        qobject_cast<View *>(views().at(i))->moveView(player);
    }
}

void Game::reset()
{

}
