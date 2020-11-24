#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "player.h"

class Game : public QGraphicsScene
{
    Q_OBJECT

public:
    Game(QGraphicsScene *parent = nullptr);
    ~Game();
    //void addItemInMap(QGraphicsItem* some);
    static QGraphicsScene *scene;
    bool start();
    void exit();


private:
    bool startBool;
    Player *player1;
    Player *player2;
    int tabScore[];

    void timer();
    void reset();



};
#endif // MAINWINDOW_H
