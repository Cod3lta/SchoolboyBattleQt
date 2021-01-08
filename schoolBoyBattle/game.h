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
#include <QElapsedTimer>

class Game : public QGraphicsScene
{
    Q_OBJECT

public:
    Game(int nbPlayers, QString terrainFileName, QGraphicsScene *parent = nullptr);
    bool start();
    void exit();
    void keyPress(QKeyEvent *event);
    void keyRelease(QKeyEvent *event);

private:
    QTimer *playerRefresh;
    QElapsedTimer *playerRefreshDelta;
    QList<Player*> players;
    QList<Candy*> candies;
    KeyInputs *keyboardInputs;

    DataLoader *dataLoader;

    bool startBool;
    int tabScore[];


    void reset();
    void playerMoveTimer();

protected:

};
#endif // MAINWINDOW_H
