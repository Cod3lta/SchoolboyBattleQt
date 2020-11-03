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
private:
    QGraphicsScene *scene;
};
#endif // MAINWINDOW_H
