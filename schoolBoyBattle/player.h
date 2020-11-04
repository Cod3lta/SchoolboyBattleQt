#ifndef PLAYER_H
#define PLAYER_H

#include <QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>

class Player : public QMainWindow
{
    Q_OBJECT

public:
    Player(QWidget *parent = nullptr);
    ~Player();
private:
    QGraphicsScene *scenePlayer;
};
#endif // MAINWINDOW_H
