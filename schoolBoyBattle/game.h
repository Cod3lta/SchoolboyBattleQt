#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>

class Game : public QMainWindow
{
    Q_OBJECT

public:
    Game(QWidget *parent = nullptr);
    ~Game();
    void addItemInMap(QGraphicsItem* some);
    static QGraphicsScene *scene;
    bool start();
    void exit();


private:
    bool startBool;
    int tabScore[];
    void timer();
    void reset();


};
#endif // MAINWINDOW_H
