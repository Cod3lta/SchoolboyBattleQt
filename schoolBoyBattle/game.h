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
#include <QElapsedTimer>

class Game : public QGraphicsScene
{
    Q_OBJECT

public:
    Game(QString terrainFileName, bool isMultiplayer, QGraphicsScene *parent = nullptr);
    bool start();
    void exit();
    void keyPress(QKeyEvent *event);
    void keyRelease(QKeyEvent *event);
    QList<Tile*> tilesNearby(QString layer, int x, int y);
    QList<Candy *> candiesNearby(int x, int y);

private:
    QTimer *playerRefresh;
    QElapsedTimer *playerRefreshDelta;
    QList<Player*> players;
    QList<Candy*> candies;
    QList<TileCandyPlacement*> tileCandyPlacements;
    // la string est le nom des layers
    QHash<QString, QList<Tile*>> tiles;
    KeyInputs *keyboardInputs;
    DataLoader *dataLoader;

    QPoint redSpawnpoint;
    QPoint blackSpawnpoint;

    bool isMultiplayer;
    bool startBool;
    int tabScore[];

    void setCustomSceneRect();
    void placeTiles();
    void reset();
    void refreshEntities();
    void placeTilesCandyPlacement();

protected:

public slots:
    void startGame(int nbPlayers);
    void spawnCandy(int x, int y, int candyType, int candySize, TileCandyPlacement* tilePlacement);

};
#endif // MAINWINDOW_H
