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

#include "tilecandyplacement.h"
#include "dataloader.h"

#define SERVER_ROLLBACK_DELAY 1000
#define REFRESH_DELAY 1/60*1000
#define PLAYER_WIDTH 120
#define PLAYER_HEIGHT 150
#define PLAYER_SPEED 8

Game::Game(QString terrainFileName, int nbPlayers, bool isMultiplayer, TcpClient *tcpClient, QGraphicsScene *parent)
    : QGraphicsScene(parent),
      tcpClient(tcpClient),
      isMultiplayer(isMultiplayer)
{

    // Chargement des données
    dataLoader = new DataLoader(terrainFileName);
    //QHashIterator<QString, QString>
    keyboardInputs = new KeyInputs(tcpClient->getSocketDescriptor());
    addItem(keyboardInputs);

    startGame(nbPlayers);
}

void Game::startGame(int nbPlayers) {
    placeTiles();
    setCustomSceneRect();
    placeTilesCandyPlacement();
    for(int i = 0; i < tileCandyPlacements.length(); i++)
        connect(tileCandyPlacements.at(i), &TileCandyPlacement::spawnCandy, this, &Game::spawnCandy);

    // Joueurs & clavier
    if(isMultiplayer) {
        setupMultiplayerGame();
    }else {
        // Créer chaque joueur
        for(int i = 0; i < nbPlayers; i++) {
            players.append(new Player(i, i%2, rand()%2, dataLoader, &tiles["4-collision"], PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_SPEED));
            addItem(players.at(i));
        }
        // Connecter les signaux de keyboardInputs aux slots des joueurs pour le clavier
        for (int i = 0; i < players.size(); ++i)
            connect(keyboardInputs, &KeyInputs::playerKeyToggle, players.at(i), &Player::keyMove);
    }


    // Refresh du déplacement des joueurs
    playerRefreshDelta = new QElapsedTimer();
    playerRefresh = new QTimer(this);
    playerRefresh->setInterval(REFRESH_DELAY);
    connect(playerRefresh, &QTimer::timeout, this, &Game::refreshEntities);
    playerRefresh->start();
    playerRefreshDelta->start();
}

void Game::setupMultiplayerGame() {
    // Mettre en place le timer de rollback
    serverRollback = new QTimer(this);
    serverRollback->setInterval(SERVER_ROLLBACK_DELAY);
    connect(serverRollback, &QTimer::timeout, this, &Game::sendRollback);
    serverRollback->start();
    connect(this, &Game::rollbackToServer, tcpClient, &TcpClient::rollback);
    // Recevoir et traiter les rollbacks
    connect(tcpClient, &TcpClient::userRollback, this, &Game::receiveRollback);

    // Créer chaque joueur présent dans la liste des joueurs de l'objet tcpClient
    QHash<int, QHash<QString, QString>> clientsList = tcpClient->getUsersList();
    QHashIterator<int, QHash<QString, QString>> i(clientsList);
    int count = 0;
    while(i.hasNext()) {
        i.next();
        QHash<QString, QString> clientProps = i.value();
        int socketDescriptor = tcpClient->getSocketDescriptor();
        players.append(new Player(
                           i.key(),
                           clientProps["team"].toInt(),
                           clientProps["gender"].toInt(),
                           dataLoader,
                           &tiles["4-collision"],
                           PLAYER_WIDTH,
                           PLAYER_HEIGHT,
                           PLAYER_SPEED));
        addItem(players.at(count));
        // Si le descriptor de l'objet qu'on a ajouté est le même que le nôtre
        if(i.key() == socketDescriptor) {
            playerIndexInMulti = count;
            // On connecte la sortie du clavier à ce joueur
            connect(keyboardInputs, &KeyInputs::playerKeyToggle, players.at(count), &Player::keyMove);
        }
        count++;
    }
    // Signal / slot du keyboard au serveur
    // Envoyer les mouvements de ce joueur au serveur
    connect(keyboardInputs, &KeyInputs::playerKeyToggle, tcpClient, &TcpClient::keyMove);
    // Traiter les mouvements reçus du serveur
    for (int i = 0; i < players.size(); ++i)
        connect(tcpClient, &TcpClient::userMove, players.at(i), &Player::keyMove);
}

void Game::sendRollback() {
    qDebug() << "Send rollback to server";
    int playerX = players.at(playerIndexInMulti)->x();
    int playerY = players.at(playerIndexInMulti)->y();
    emit rollbackToServer(playerX, playerY);
}

void Game::receiveRollback(int playerX, int playerY, int playerDescriptor) {
    for(int i = 0; i < players.size(); i++) {
        if(players.at(i)->getId() == playerDescriptor) {
            players.at(i)->setPos(playerX, playerY);
        }
    }
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

void Game::placeTilesCandyPlacement() {
    DataLoader::TileLayerStruct* candyPlacementsLayer = dataLoader->tileLayers["6-candy-placements"];

    for(int y = 0; y < candyPlacementsLayer->tiles.size(); y++) {
        for(int x = 0; x < candyPlacementsLayer->tiles.at(y).size(); x++) {
            int tileType = candyPlacementsLayer->tiles.at(y).at(x);
            if(tileType != 0) {
                TileCandyPlacement *candyPlacement = new TileCandyPlacement(
                            dataLoader->getCandyRessources(tileType)->respawnDelayMs,
                            x,
                            y,
                            candyPlacementsLayer,
                            "6-candy-placements",
                            tileType,
                            dataLoader);
                tileCandyPlacements.append(candyPlacement);
                addItem(candyPlacement);
            }
        }
    }
}

/*
 * Va créer et placer sur la scène toutes les tiles de toutes les layers
 * sauf celles qui sont sur la layer "6-candy-placements"
 * Ces tiles seront placées dans la fonction placeCandyPlacements()
 */
void Game::placeTiles() {
    QMap<QString, DataLoader::TileLayerStruct*> layers = dataLoader->tileLayers;
    QMapIterator<QString, DataLoader::TileLayerStruct*> layersIterator(layers);
    while (layersIterator.hasNext()) {
        layersIterator.next();
        if(layersIterator.key() != "6-candy-placements") {
            QList<Tile*> tilesList;
            DataLoader::TileLayerStruct* value = layersIterator.value();
            QString key = layersIterator.key();
            for(int y = 0; y < value->tiles.size(); y++) {
                for(int x = 0; x < value->tiles.at(y).size(); x++) {
                    int tileType = value->tiles.at(y).at(x);
                    if(tileType != 0) {
                        Tile *tile = new Tile(x, y, value, key, tileType, dataLoader);
                        tilesList.append(tile);
                        addItem(tile);
                    }
                }
            }
            tiles.insert(key, tilesList);
        }
    }
}

QList<Tile*> Game::tilesNearby(QString layer, int x, int y) {
    QList<Tile*> tilesNearby;
    for(int i = 0; i < tiles[layer].size(); i++) {
        Tile *tile = tiles[layer].at(i);
        // Sélectionner les tiles à proximité du point (x,y)
        int tileSize = dataLoader->getTileSize();
        if(
                tile->x() > x - 2 * tileSize &&
                tile->x() < x + 2 * tileSize &&
                tile->y() > y - 2 * tileSize &&
                tile->y() < y + 2 * tileSize) {
            tilesNearby.append(tile);
        }
    }
    return tilesNearby;
}

QList<Candy*> Game::candiesNearby(int x, int y) {
    QList<Candy*> candiesNearby;
    for(int i = 0; i < candies.size(); i++) {
        Candy *candy = candies.at(i);
        // Sélectionner les candy à proximité du point (x,y)
        int tileSize = dataLoader->getTileSize();
        if(
                candy->x() > x - 2 * tileSize &&
                candy->x() < x + 2 * tileSize &&
                candy->y() > y - 2 * tileSize &&
                candy->y() < y + 2 * tileSize) {
            candiesNearby.append(candy);
        }
    }
    return candiesNearby;
}

void Game::refreshEntities() {
    if(views().length() == 0)
        return;
    int delta=playerRefreshDelta->nsecsElapsed();
    // Refresh le joueur
    playerRefreshDelta->restart();
    for(int i = 0; i < players.size(); i++) {
        Player *player = players.at(i);
        player->refresh(delta);
        if(!isMultiplayer)
            qobject_cast<View *>(this->views().at(i))->moveView(player, PLAYER_WIDTH, PLAYER_HEIGHT);
    }
    if(isMultiplayer)
        qobject_cast<View *>(this->views().at(0))->moveView(players.at(playerIndexInMulti), PLAYER_WIDTH, PLAYER_HEIGHT);
}

void Game::spawnCandy(int x, int y, int candyType, int candySize, TileCandyPlacement* tilePlacement) {
    Candy *candy = new Candy(x, y, candyType, candySize, dataLoader, tilePlacement);
    addItem(candy);
    candies.append(candy);
}


void Game::reset() {

}
