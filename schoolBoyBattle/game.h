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

class Game : public QGraphicsScene
{
    Q_OBJECT

public:
    Game(QGraphicsScene *parent = nullptr);
    bool start();
    void exit();
    void keyPress(QKeyEvent *event);
    void keyRelease(QKeyEvent *event);

private:
    QTimer *timer;
    bool startBool;
    Player *player1;
    Player *player2;
    KeyInputs *keyboardInputs;
    int tabScore[];

    void reset();
    void playerMove();

protected:

};
#endif // MAINWINDOW_H
