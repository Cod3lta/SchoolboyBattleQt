#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QSet>
#include <QTimer>
#include "player.h"
#include "keyinputs.h"
#include <QElapsedTimer>

class Game : public QGraphicsScene
{
    Q_OBJECT

public:
    Game(int nbPlayers, QGraphicsScene *parent = nullptr);
    bool start();
    void exit();
    void keyPress(QKeyEvent *event);
    void keyRelease(QKeyEvent *event);

private:
    QTimer *playerRefresh;
    QElapsedTimer *playerRefreshDelta;
    QList<Player*> players;
    KeyInputs *keyboardInputs;

    bool startBool;
    int tabScore[];


    void reset();
    void playerMoveTimer();

protected:

};
#endif // MAINWINDOW_H
