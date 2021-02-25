/*
 * Description : Cette classe hérite de QGraphicsScene et s’occupe de la création,
 *               du déroulement et de la suppression de chaque partie.
 *               Elle crée tous les QGraphicsItem nécessaires, les ajoute à la scène
 *               et les connecte entre eux pour leur fonctionnement.
 *               Cette classe s’occupe également d’envoyer et recevoir des signaux
 *               à la classe TcpClient lorsqu’il faut communiquer avec le serveur.
 * Version     : 1.0.0
 * Date        : 25.01.2021
 * Auteurs     : Prétat Valentin, Badel Kevin et Margueron Yasmine
*/

#include <QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QSet>
#include <QTimer>
#include "candy.h"
#include "dataloader.h"
#include "player.h"
#include "keyinputs.h"
#include "dataloader.h"
#include "tile.h"
#include "tilecandyplacement.h"
#include "tcpclient.h"
#include <QElapsedTimer>

#ifndef GAME_H
#define GAME_H

class Game : public QGraphicsScene
{
    Q_OBJECT

public:
    Game(QGraphicsScene *parent = nullptr);
    ~Game();
    void keyPress(QKeyEvent *event);
    void keyRelease(QKeyEvent *event);
    QList<Tile*> tilesNearby(QString layer, int x, int y);
    QList<Candy *> candiesNearby(int x, int y);
    QList<TileCandyPlacement *> getTileCandyPlacementList();
    bool hasPlayerAnyCandyValid(int playerId);

    enum PlayerMovesEnum : int {up = 0, right = 1, down = 2, left = 3};

private:
    TcpClient *tcpClient;
    QTimer *playerRefresh;
    QElapsedTimer *playerRefreshDelta;
    QTimer *serverRollback;
    QTimer *gameTimer;
    QHash<int, Player*> players;
    QHash<int, Candy*> candies;
    QList<TileCandyPlacement*> tileCandyPlacements;
    // la string est le nom des layers
    QHash<QString, QList<Tile*>> tiles;
    KeyInputs *keyboardInputs;
    DataLoader *dataLoader;

    bool startBool;
    QHash<int, int> scores;

    void setCustomSceneRect();
    void placeTiles();
    void refreshEntities();
    void placeTilesCandyPlacement();
    void setupMultiplayerGame();
    void setupLocalGame(int nbPlayers);
    void placeBosses();

private slots:
    void sendRollback();
    void receiveRollback(double playerX, double playerY, QHash<int, QPointF> candies, int playerDescriptor);
    void spawnCandy(int candyType, int candySize, int nbPoints, int tilePlacementId, int candyId);
    void playerStealsCandies(int candyIdStartingFrom, int playerWinningId);
    void playerValidateCandies(int playerId);
    void playerPickedUpCandyMulti(int descriptor, int candyId);
    void deleteCandy(int id, int playerId);
    void gameEnd();

public slots:
    void startGame(QString terrainFileName, int nbPlayers, bool isMultiplayer, TcpClient *tcpClient);

signals:
    void rollbackToServer(QPointF playerPos, QHash<int, QPointF> candiesTaken);
    void playerStealCandies(int candyIdStartingFrom, int playerWinningId);
    void teamsPointsChanged(int nbPointsRed, int nbPointsBlack);
    void showEndScreen(int teamWinner);

};
#endif // MAINWINDOW_H
