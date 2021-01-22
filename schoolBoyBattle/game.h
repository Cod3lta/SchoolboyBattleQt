#ifndef GAME_H
#define GAME_H

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

class Game : public QGraphicsScene
{
    Q_OBJECT

public:
    Game(QGraphicsScene *parent = nullptr);
    ~Game();
    bool start();
    void exit();
    void keyPress(QKeyEvent *event);
    void keyRelease(QKeyEvent *event);
    QList<Tile*> tilesNearby(QString layer, int x, int y);
    QList<Candy *> candiesNearby(int x, int y);
    QList<TileCandyPlacement *> getTileCandyPlacementList();
    bool hasPlayerAnyCandyValid(int playerId);

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
    int playerIndexInMulti;         // position du joueur actuel dans la liste "players" si
                                    // on est en multijoueur
    QHash<int, int> scores;

    void setCustomSceneRect();
    void placeTiles();
    void reset();
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
