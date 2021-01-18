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
    dataLoader = new DataLoader(terrainFileName, isMultiplayer);
    //QHashIterator<QString, QString>
    keyboardInputs = new KeyInputs(tcpClient->getSocketDescriptor());
    addItem(keyboardInputs);

    startGame(nbPlayers);
}

void Game::startGame(int nbPlayers) {
    placeTiles();
    setCustomSceneRect();
    placeTilesCandyPlacement();

    // Joueurs & clavier
    if(dataLoader->isMultiplayer()) {
        setupMultiplayerGame();
    }else {
        // Connecter les signaux des placements de candy à la fonction qui les créé
        for(int i = 0; i < tileCandyPlacements.length(); i++)
            connect(tileCandyPlacements.at(i), &TileCandyPlacement::spawnCandy, this, &Game::spawnCandy);

        // Créer chaque joueur
        for(int i = 0; i < nbPlayers; i++) {
            players.insert(i, new Player(i, i%2, rand()%2, dataLoader, &tiles["4-collision"], PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_SPEED));
            addItem(players.value(i));
        }
        // Connecter les signaux de keyboardInputs aux slots des joueurs pour le clavier
        QHashIterator<int, Player*> i(players);
        while(i.hasNext()) {
            i.next();
            connect(keyboardInputs, &KeyInputs::playerKeyToggle, i.value(), &Player::keyMove);
        }
    }

    // Signaux slots quand un joueur vole le candy d'un autre joueur
    QHashIterator<int, Player*> i(players);
    while(i.hasNext()) {
        i.next();
        // Signal qui est émit quand ce joueur (i.value()) vole des candies à d'autres joueurs
        connect(i.value(), &Player::stalsCandies, this, &Game::playerStealsCandies);
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
    // Recevoir les joueur qui prennent des candies libres
    connect(tcpClient, &TcpClient::playerPickUpCandy, this, &Game::playerPickedUpCandyMulti);

    // Créer chaque joueur présent dans la liste des joueurs de l'objet tcpClient
    QHash<int, QHash<QString, QString>> clientsList = tcpClient->getUsersList();
    QHashIterator<int, QHash<QString, QString>> i(clientsList);
    int count = 0;
    int socketDescriptor = tcpClient->getSocketDescriptor();
    while(i.hasNext()) {
        i.next();
        QHash<QString, QString> clientProps = i.value();
        players.insert(i.key(), new Player(
                           i.key(),
                           clientProps["team"].toInt(),
                           clientProps["gender"].toInt(),
                           dataLoader,
                           &tiles["4-collision"],
                           PLAYER_WIDTH,
                           PLAYER_HEIGHT,
                           PLAYER_SPEED));
        addItem(players.value(i.key()));
        // Si le descriptor de l'objet qu'on a ajouté est le même que le nôtre
        if(i.key() == socketDescriptor) {
            playerIndexInMulti = i.key();
            // On connecte la sortie du clavier à ce joueur
            connect(keyboardInputs, &KeyInputs::playerKeyToggle, players.value(i.key()), &Player::keyMove);
            // On connecte la détection des candy au serveur (demander au serveur si un candy est libre)
            connect(players.value(i.key()), &Player::isCandyFree, tcpClient, &TcpClient::isCandyFree);
        }
        count++;
    }
    // Signal / slot du keyboard au serveur
    // Envoyer les mouvements de ce joueur au serveur
    connect(keyboardInputs, &KeyInputs::playerKeyToggle, tcpClient, &TcpClient::keyMove);
    // Traiter les mouvements reçus du serveur
    QHashIterator<int, Player*> j(players);
    while(j.hasNext()) {
        j.next();
        connect(tcpClient, &TcpClient::userMove, j.value(), &Player::keyMove);
    }

    // Si notre instance est le candy master
    if(tcpClient->isCandyMaster()) {
        for(int i = 0; i < tileCandyPlacements.length(); i++) {
            // Connecter le signal de création de candy au slot de cette classe
            connect(tileCandyPlacements.at(i), &TileCandyPlacement::spawnCandy, this, &Game::spawnCandy);
            // Envoyer les signaux de création des candy au serveur pour les broadcaster ensuite
            connect(tileCandyPlacements.at(i), &TileCandyPlacement::spawnCandy, tcpClient, &TcpClient::sendNewCandy);
        }
    }else{
        // Connecter les messages du serveur à la création des nouveaux candy
        connect(tcpClient, &TcpClient::spawnNewCandy, this, &Game::spawnCandy);
    }
}

/*
 * Toutes les x milisecondes, envoie au serveur les infos
 * de l'emplacement du joueur et des candy qu'il a ramassé
 */
void Game::sendRollback() {
    int playerX = players.value(playerIndexInMulti)->x();
    int playerY = players.value(playerIndexInMulti)->y();
    emit rollbackToServer(playerX, playerY);
}

/*
 * Traîte les rollbacks envoyés par les autres joueurs en
 * déplaçant les items là où ils sont indiqués dans le message
 */
void Game::receiveRollback(int playerX, int playerY, int playerDescriptor) {
    players.value(playerDescriptor)->setPos(playerX, playerY);
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

    int count = 0;
    for(int y = 0; y < candyPlacementsLayer->tiles.size(); y++) {
        for(int x = 0; x < candyPlacementsLayer->tiles.at(y).size(); x++) {
            int tileType = candyPlacementsLayer->tiles.at(y).at(x);
            if(tileType != 0) {
                TileCandyPlacement *candyPlacement = new TileCandyPlacement(
                            count,
                            dataLoader->getCandyRessources(tileType)->respawnDelayMs,
                            x,
                            y,
                            candyPlacementsLayer,
                            "6-candy-placements",
                            tileType,
                            dataLoader);
                tileCandyPlacements.append(candyPlacement);
                addItem(candyPlacement);
                count++;
            }
        }
    }
}

/*
 * Va créer et placer sur la scène toutes les tiles de toutes les layers
 * sauf celles qui sont sur la layer "6-candy-placements"
 * Ces tiles seront placées dans la fonction placeTilesCandyPlacement()
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
    QHashIterator<int, Candy*> i(candies);
    while(i.hasNext()) {
        i.next();
        Candy *candy = i.value();
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
    if(views().length() == 0) return;
    int delta = playerRefreshDelta->nsecsElapsed();

    // Refresh le joueur
    playerRefreshDelta->restart();
    QHashIterator<int, Player*> i(players);
    int count = 0;
    while(i.hasNext()) {
        i.next();
        i.value()->refresh(delta);

        if(!candies.isEmpty()) {
            // Refresh les candies capturés par ce joueur
            QList<int> candiesTaken = i.value()->getCandiesTaken();
            if(!candiesTaken.isEmpty()) {
                // le 1er candy de la liste suit le joueur
                candies[candiesTaken.first()]->refresh(i.value()->pos(), 0);
                Candy *previousCandy = candies[candiesTaken.first()];
                for(int i = 0; i < candiesTaken.length(); i++) {
                    candies[candiesTaken.at(i)]->refresh(previousCandy->pos(), i);
                    previousCandy = candies[candiesTaken.at(i)];
                }
            }
        }

        // Refresh les vues de chaque joueur
        if(!dataLoader->isMultiplayer())
            qobject_cast<View *>(this->views().at(count))->moveView(i.value(), PLAYER_WIDTH, PLAYER_HEIGHT);
        count++;
    }

    // Refresh la vue du programme
    if(dataLoader->isMultiplayer())
        qobject_cast<View *>(this->views().at(0))->moveView(players.value(playerIndexInMulti), PLAYER_WIDTH, PLAYER_HEIGHT);
}

void Game::spawnCandy(int candyType, int candySize, int tilePlacementId, int candyId) {
    TileCandyPlacement* tileCandyPlacementToSpawn = tileCandyPlacements.at(tilePlacementId);
    Candy *candy = new Candy(candyType, candySize, dataLoader, tileCandyPlacementToSpawn, candyId);
    addItem(candy);
    candies.insert(candyId, candy);
}

QList<int> Game::playerStealsCandies(int candyIdStartingFrom, int playerWinningId) {
    Player *playerLosingCandies = players[candies[candyIdStartingFrom]->getCurrentPlayerId()];
    QList<int>candiesGained = playerLosingCandies->looseCandies(candyIdStartingFrom);
    // définir le nouveau joueur pour chacun de ces candy
    for(int i = 0; i < candiesGained.length(); i++)
        candies[candiesGained.at(i)]->setCurrentPlayerId(playerWinningId);
    return candiesGained;
}

/*
 * Slot qui n'est utilisé qu'en multijoueur, s'active quand un joueur ramasse
 * un candy qui n'appartenait à personne jusque là
 */
void Game::playerPickedUpCandyMulti(int descriptor, int candyId) {
    // Dire au candy qu'il a été ramassés par un joueur
    candies[candyId]->pickUp(descriptor);
    // Ajouter le candy à la liste des candies du joueur
    players[descriptor]->pickupCandyMulti(candyId);
}

/*void Game::sendCandyToServer(int x, int y, int candyType, int candySize, int candyPlacementId) {

}*/


void Game::reset() {

}

QList<TileCandyPlacement *> Game::getTileCandyPlacementList() {
    return tileCandyPlacements;
}
