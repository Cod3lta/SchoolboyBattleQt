#include "game.h"
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QSet>
#include <QMessageBox>
#include <QSound>

#include "candy.h"
#include "player.h"
#include "keyinputs.h"
#include "view.h"
#include "tile.h"

#include "tilecandyplacement.h"
#include "dataloader.h"
#include "boss.h"

#define SERVER_ROLLBACK_DELAY 1000
#define REFRESH_DELAY 1/60*1000                 // Pour avoir un taux de refresh atteignant 60 images / secondes

Game::Game(QGraphicsScene *parent)
    : QGraphicsScene(parent)
{}

void Game::startGame(QString terrainFileName, int nbPlayers, bool isMultiplayer, TcpClient *tcpClient) {

    this->tcpClient = tcpClient;
    // Chargement des données
    dataLoader = new DataLoader(terrainFileName, isMultiplayer);
    //QHashIterator<QString, QString>
    keyboardInputs = new KeyInputs(tcpClient->getSocketDescriptor());
    addItem(keyboardInputs);

    // Préparer le tableau des scores
    scores.insert(0, 0);
    scores.insert(1, 0);

    placeTiles();
    setCustomSceneRect();
    placeTilesCandyPlacement();
    placeBosses();

    // Faire un setup spécifique à chaque mode de jeu
    if(dataLoader->isMultiplayer())
        setupMultiplayerGame();
    else
        setupLocalGame(nbPlayers);

    // Refresh du déplacement des joueurs
    playerRefreshDelta = new QElapsedTimer();
    playerRefresh = new QTimer(this);
    playerRefresh->setInterval(REFRESH_DELAY);
    connect(playerRefresh, &QTimer::timeout, this, &Game::refreshEntities);
    playerRefresh->start();
    playerRefreshDelta->start();
    gameTimer = new QTimer(this);
    gameTimer->singleShot(3 * 60 * 1000, this, &Game::gameEnd);
}

void Game::setupLocalGame(int nbPlayers) {
    // Connecter les signaux des placements de candy à la fonction qui les créé
    for(int j = 0; j < tileCandyPlacements.length(); j++) {
        TileCandyPlacement *candyToConnect = tileCandyPlacements.at(j);
        connect(candyToConnect, &TileCandyPlacement::spawnCandy, this, &Game::spawnCandy);
    }

    // Créer chaque joueur
    for(int i = 0; i < nbPlayers; i++) {
        players.insert(i, new Player(i, i%2, rand()%2, "", dataLoader, &tiles["4-collision"]));
        addItem(players.value(i));
    }

    // Pour chaque joueur
    QHashIterator<int, Player*> i(players);
    while(i.hasNext()) {
        i.next();
        // Connecter les signaux de keyboardInputs aux slots des joueurs pour le clavier
        connect(keyboardInputs, &KeyInputs::playerKeyToggle, i.value(), &Player::keyMove);
        // Signal qui est émit quand ce joueur (i.value()) vole des candies à d'autres joueurs
        connect(i.value(), &Player::stealCandies, this, &Game::playerStealsCandies);
        // Signal qui est émit quand ce joueur valide ses candies
        connect(i.value(), &Player::validateCandies, this, &Game::playerValidateCandies);
    }
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
    // Recevoir les candy que tel joueur vol
    connect(tcpClient, &TcpClient::playerStealCandy, this, &Game::playerStealsCandies);
    // Recevoir les candy que tel joueur valide
    connect(tcpClient, &TcpClient::playerValidateCandy, this, &Game::playerValidateCandies);

    // Créer chaque joueur présent dans la liste des joueurs de l'objet tcpClient
    QHash<int, QHash<QString, QString>> clientsList = tcpClient->getUsersList();
    int count = 0;
    int socketDescriptor = tcpClient->getSocketDescriptor();
    QHashIterator<int, QHash<QString, QString>> i(clientsList);
    while(i.hasNext()) {
        i.next();
        QHash<QString, QString> clientProps = i.value();
        if(i.key() == socketDescriptor)
            dataLoader->setPlayerIndexInMulti(i.key());
        players.insert(i.key(), new Player(i.key(), clientProps["team"].toInt(), clientProps["gender"].toInt(), clientProps["username"], dataLoader, &tiles["4-collision"]));
        addItem(players.value(i.key()));
        // Si le descriptor de l'objet qu'on a ajouté est le même que le nôtre
        if(i.key() == socketDescriptor) {
            players[i.key()]->setMainPlayerInMulti();
            // On connecte la sortie du clavier à ce joueur
            connect(keyboardInputs, &KeyInputs::playerKeyToggle, players.value(i.key()), &Player::keyMove);
            // On connecte la détection des candy au serveur (demander au serveur si un candy est libre)
            connect(players.value(i.key()), &Player::isCandyFree, tcpClient, &TcpClient::isCandyFree);
            // Signal qui est émit quand ce joueur (i.value()) vole des candies à d'autres joueurs
            // Envoyer l'info aux autres joueurs
            // A FAIRE QUAND ON A LA CONFIRMATION (QUEUEPROTECTED) QUE LES CANDIES PEUVENT ETRE VOLES
            connect(this, &Game::playerStealCandies, tcpClient, &TcpClient::playerStealsCandies);
            // Voler le candy pour cette instance
            connect(players.value(i.key()), &Player::stealCandies, this, &Game::playerStealsCandies);
//             Pour qu'un player puisse demander si tous ses candies sont déjà validés
//            connect(players.value(i.key()), &Player::arePlayerTakenCandiesValidated, this, &Game::arePlayerTakenCandiesValidated);
            // Envoyer l'info au serveur que ce joueur a validé ses candies
            connect(players.value(i.key()), &Player::validateCandies, tcpClient, &TcpClient::playerValidateCandies);
            // Signal qui est émit quand ce joueur valide ses candies
            connect(players.value(i.key()), &Player::validateCandies, this, &Game::playerValidateCandies);
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
    Player *player = players.value(dataLoader->getPlayerIndexInMulti());
    QHash<int, QPointF> candiesTakenToSend;
    QList<int> candiesTaken = players[dataLoader->getPlayerIndexInMulti()]->getCandiesTaken();
    for(int i = 0; i < candiesTaken.length(); i++) {
        candiesTakenToSend.insert(
                    candiesTaken.at(i),
                    candies[candiesTaken.at(i)]->pos());
    }
    emit rollbackToServer(player->pos(), candiesTakenToSend);
}

/*
 * Traîte les rollbacks envoyés par les autres joueurs en
 * déplaçant les items là où ils sont indiqués dans le message
 */
void Game::receiveRollback(double playerX, double playerY, QHash<int, QPointF> candies, int playerDescriptor) {
    players.value(playerDescriptor)->setPos(playerX, playerY);
    QHashIterator<int, QPointF> i(candies);
    while(i.hasNext()) {
        i.next();
        if(candies.contains(i.key()))
            this->candies[i.key()]->setPos(i.value());
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
        if(dataLoader->getTileRessource(tiles["5-config"].at(i)->getTileType())->name == "world/config/scene-rect-top-left.png") {
            customSceneRect.setX(tiles["5-config"].at(i)->x());
            customSceneRect.setY(tiles["5-config"].at(i)->y());
            continue;
        }

        if(dataLoader->getTileRessource(tiles["5-config"].at(i)->getTileType())->name == "world/config/scene-rect-bottom-right.png") {
            customSceneRect.setWidth(tiles["5-config"].at(i)->x() - customSceneRect.x());
            customSceneRect.setHeight(tiles["5-config"].at(i)->y() - customSceneRect.y());
            break;
        }
    }
    setSceneRect(customSceneRect);
}

void Game::placeBosses() {
    for(int i = 0; i < tiles.value("5-config").size(); i++) {
        if(dataLoader->getTileRessource(tiles["5-config"].at(i)->getTileType())->name == "world/config/boss-black.png") {
            // Créer le père fouettard
            addItem(new Boss(1, tiles["5-config"].at(i)->x(), tiles["5-config"].at(i)->y(), dataLoader));
            continue;
        }

        if(dataLoader->getTileRessource(tiles["5-config"].at(i)->getTileType())->name == "world/config/boss-red.png") {
            // Créer le st-nicholas
            addItem(new Boss(0, tiles["5-config"].at(i)->x(), tiles["5-config"].at(i)->y(), dataLoader));
            continue;
        }
    }
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
                //QSize tileSize = dataLoader->getTileRessource(tiles[layer].at(i)->getTileType())->image->size();
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
            if(candy->isValidated()) continue;
            candiesNearby.append(candy);
        }
    }
    return candiesNearby;
}

void Game::refreshEntities() {
    if(views().length() == 0) return;
    int delta = playerRefreshDelta->nsecsElapsed();
    double deltaMs = delta/10e6;

    // Refresh le joueur
    playerRefreshDelta->restart();
    QHashIterator<int, Player*> i(players);
    int count = 0;
    while(i.hasNext()) {
        i.next();
        i.value()->refresh(deltaMs, tcpClient->getSocketDescriptor());

        if(!candies.isEmpty()) {
            // Refresh les candies capturés par ce joueur
            QList<int> candiesTaken = i.value()->getCandiesTaken();
            if(!candiesTaken.isEmpty()) {
                candies[candiesTaken.first()]->refresh(i.value()->pos(), 0, deltaMs);
                Candy *previousCandy = candies[candiesTaken.first()];
                // pour chaque bonbon pris par ce joueur
                for(int i = 0; i < candiesTaken.length(); i++) {
                    // Si le candy n'existe pas (plus), on passe au prochain
                    if(!candies.contains(candiesTaken.at(i))) continue;
                    if(candies[candiesTaken.at(i)]->isValidated()) {
                        // Animation des candy vers le point de spawn
                        candies[candiesTaken.at(i)]->capture(deltaMs);
                    }else{
                        // On déplace les candy
                        // le 1er candy de la liste suit le joueur
                        candies[candiesTaken.at(i)]->refresh(previousCandy->pos(), i, deltaMs);
                        previousCandy = candies[candiesTaken.at(i)];
                    }
                }
            }
        }

        // Refresh les vues de chaque joueur
        if(!dataLoader->isMultiplayer())
            qobject_cast<View *>(this->views().at(count))->moveView(
                        i.value(),
                        dataLoader->getPlayerSize().x(),
                        dataLoader->getPlayerSize().y(),
                        deltaMs);
        count++;
    }

    // Refresh la vue du programme
    if(dataLoader->isMultiplayer())
        qobject_cast<View *>(this->views().at(0))->moveView(
                    players.value(dataLoader->getPlayerIndexInMulti()),
                    dataLoader->getPlayerSize().x(),
                    dataLoader->getPlayerSize().y(),
                    deltaMs);
}

void Game::spawnCandy(int candyType, int candySize, int nbPoints, int tilePlacementId, int candyId) {
    TileCandyPlacement* tileCandyPlacementToSpawn = tileCandyPlacements.at(tilePlacementId);
    Candy *candy = new Candy(candyType, candySize, nbPoints, dataLoader, tileCandyPlacementToSpawn, candyId);
    connect(candy, &Candy::validated, this, &Game::deleteCandy);
    addItem(candy);
    candies.insert(candyId, candy);
}

void Game::playerStealsCandies(int candyIdStartingFrom, int playerWinningId) {
    // Si le candy n'existe plus, on annule
    if(!candies.contains(candyIdStartingFrom)) return;
    Player *victim = players[candies[candyIdStartingFrom]->getCurrentPlayerId()];
    Player *stealer = players[playerWinningId];
    // S'ils sont de la même équipe, on annule
    if(victim->getTeam() == stealer->getTeam()) return;
    QList<int>candiesGained = victim->looseCandies(candyIdStartingFrom);

    // Si le candy est déjà validé dans une équipe, on annule
    if(candies[candyIdStartingFrom]->isValidated()) return;

    // S'il n'y a pas de candy volé, on s'arrête là
    if(candiesGained.length() <= 0) return;

    // Si on est en multijoueur, confirmer aux autres clients que des candies
    // ont été volés
    emit playerStealCandies(candyIdStartingFrom, playerWinningId);

    // définir le nouveau joueur pour chacun de ces candy
    for(int i = 0; i < candiesGained.length(); i++) {
        candies[candiesGained.at(i)]->setCurrentPlayerId(playerWinningId);
        candies[candiesGained.at(i)]->setTeamId(stealer->getTeam());
    }

    // Ajouter tous les candies volés dans la queue du joueur
    stealer->prependCandiesTaken(candiesGained);

    // Protéger la queue du joueur
    stealer->protectQueue();
}

void Game::playerValidateCandies(int playerId) {
    QList<int> candiesToValidate = players[playerId]->getCandiesTaken();
    // Valider chacun des candies de ce player
    for(int i = 0; i < candiesToValidate.length(); i++) {
        if(!candies[candiesToValidate.at(i)]->isValidated()) {
            candies[candiesToValidate.at(i)]->validate();
            scores[players[playerId]->getTeam()] += candies[candiesToValidate.at(i)]->getNbPoints();
            emit teamsPointsChanged(scores[0], scores[1]);
        }
    }
}

/*
 * Slot qui n'est utilisé qu'en multijoueur, s'active quand un joueur ramasse
 * un candy qui n'appartenait à personne jusque là
 */
void Game::playerPickedUpCandyMulti(int descriptor, int candyId) {
    // Dire au candy qu'il a été ramassés par un joueur
    candies[candyId]->pickUp(descriptor, players[descriptor]->getTeam());
    // Ajouter le candy à la liste des candies du joueur
    players[descriptor]->pickupCandyMulti(candyId);
}

void Game::deleteCandy(int id, int playerId) {
    players[playerId]->deleteCandy(id);
    candies.remove(id);

}

void Game::gameEnd() {
    delete playerRefresh;
    delete playerRefreshDelta;
    if(dataLoader->isMultiplayer()) delete serverRollback;
    delete gameTimer;
    QHashIterator<int, Player*> i(players);
    while(i.hasNext()) {
        i.next();
        delete players[i.key()];
    }
    players.clear();
    QHashIterator<int, Candy*> j(candies);
    while(j.hasNext()) {
        j.next();
        delete candies[j.key()];
    }
    candies.clear();
    for(int k = 0; k < tileCandyPlacements.length(); k++) {
        delete tileCandyPlacements.at(k);
    }
    tileCandyPlacements.clear();
    delete keyboardInputs;
    delete dataLoader;
    int teamWinner = -1;
    if(scores[0] > scores[1])
        teamWinner = 0;
    if(scores[0] < scores[1])
        teamWinner = 1;
    emit showEndScreen(teamWinner);
}

bool Game::hasPlayerAnyCandyValid(int playerId) {
    QList<int> playerCandies = players[playerId]->getCandiesTaken();
    for(int i = 0; i < playerCandies.length(); i++)
        // Si au moins un candy est validé, on retourne true
        if(candies[playerCandies.at(i)]->isValidated())
            return true;
    return false;
}

void Game::reset() {

}

QList<TileCandyPlacement *> Game::getTileCandyPlacementList() {
    return tileCandyPlacements;
}

Game::~Game() {
    delete playerRefreshDelta;
}
