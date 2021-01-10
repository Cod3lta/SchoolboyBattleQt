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
#include "tile.h"

#include "dataloader.h"
#define REFRESH_DELAY 1/60*1000
#define PLAYER_WIDTH 120
#define PLAYER_HEIGHT 150
#define PLAYER_SPEED 8

Game::Game(int nbPlayers, QString terrainFileName, QGraphicsScene *parent)
    : QGraphicsScene(parent)
{

    // Chargement des données
    dataLoader = new DataLoader(terrainFileName);

    //QPixmap background(":/Resources/background/terrain.png");
    //setBackgroundBrush(background);
    //setSceneRect(background.rect());

    // Refresh du déplacement des joueurs
    playerRefreshDelta = new QElapsedTimer();
    playerRefresh = new QTimer(this);
    playerRefresh->setInterval(REFRESH_DELAY);
    connect(playerRefresh, &QTimer::timeout, this, &Game::playerMoveTimer);
    playerRefresh->start();
    playerRefreshDelta->start();

    placeTiles();
    setCustomSceneRect();
    placeCandies();


    // Joueurs
    for(int i = 0; i < nbPlayers; i++) {
        players.append(new Player(i, i%2, dataLoader, &tiles["4-collision"], PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_SPEED));
        addItem(players.at(i));
    }

    keyboardInputs = new KeyInputs();

    // Connecter les signaux de keyboardInputs aux slots des joueurs pour le clavier
    for (int i = 0; i < players.size(); ++i) {
        connect(keyboardInputs, &KeyInputs::playerKeyToggle, players.at(i), &Player::keyMove);
    }

    addItem(keyboardInputs);
}

void Game::keyPress(QKeyEvent *event) {
    keyboardInputs->keyPress(event);
}

void Game::keyRelease(QKeyEvent *event) {
    keyboardInputs->keyRelease(event);
}

void Game::setCustomSceneRect() {
    QRectF customSceneRect;
    for(int i = 0; i < tiles.value("5-config").size(); i++) {
        if(dataLoader->getTileRessource(tiles["5-config"].at(i)->tileType)->name == "world/config/scene-rect-top-left.png") {
            customSceneRect.setX(tiles["5-config"].at(i)->x());
            customSceneRect.setY(tiles["5-config"].at(i)->y());
            continue;
        }

        if(dataLoader->getTileRessource(tiles["5-config"].at(i)->tileType)->name == "world/config/scene-rect-bottom-right.png") {
            customSceneRect.setWidth(tiles["5-config"].at(i)->x() - customSceneRect.x());
            customSceneRect.setHeight(tiles["5-config"].at(i)->y() - customSceneRect.y());
            break;
        }
    }
    setSceneRect(customSceneRect);
}

void Game::placeCandies() {
    for(int i = 0; i < tiles.value("6-candy-placements").size(); i++) {
        Tile *tile = tiles.value("6-candy-placements").at(i);
        Candy *candy = new Candy(
                    dataLoader->getCandyRessources(tile->tileType)->candyType,
                    dataLoader->getCandyRessources(tile->tileType)->candySize,
                    dataLoader);
        candies.append(candy);
        addItem(candy);
    }
}

void Game::placeTiles() {
    QMap<QString, DataLoader::TileLayerStruct*> layers = dataLoader->tileLayers;
    QMapIterator<QString, DataLoader::TileLayerStruct*> layersIterator(layers);
    while (layersIterator.hasNext()) {
        layersIterator.next();
        QList<Tile*> tilesList;
        DataLoader::TileLayerStruct* value = layersIterator.value();
        QString key = layersIterator.key();
        for(int y = 0; y < value->tiles.size(); y++) {
            for(int x = 0; x < value->tiles.at(y).size(); x++) {
                int type = value->tiles.at(y).at(x);
                if(type != 0) {
                    Tile *tile = new Tile(x, y, value->topLeftX, value->topLeftY, key, type, dataLoader);
                    tilesList.append(tile);
                    addItem(tile);
                }
            }
        }
        tiles.insert(key, tilesList);
    }
}

QList<Tile*> Game::collisionTilesNearby(int x, int y) {
    QList<Tile*> tilesNearby;
    for(int i = 0; i < tiles["4-collision"].size(); i++) {
        Tile *tile = tiles["4-collision"].at(i);
        // TODO : remplacer le 130 par la constante de taille des tiles
        if(tile->x() > x - 2 * 130 && tile->x() < x + 2 * 130 &&
                tile->y() > y - 2 * 130 && tile->y() < y + 2 * 130) {
            tilesNearby.append(tile);
        }
    }
    return tilesNearby;
}

void Game::playerMoveTimer() {
    int delta=playerRefreshDelta->nsecsElapsed();
    playerRefreshDelta->restart();
    for (int i = 0; i < players.size(); ++i) {
        Player *player = players.at(i);
        player->refresh(delta);
        qobject_cast<View *>(views().at(i))->moveView(player, PLAYER_WIDTH, PLAYER_HEIGHT);
    }
}

void Game::reset() {

}
