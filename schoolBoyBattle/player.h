#ifndef PLAYER_H
#define PLAYER_H

#include <QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>



class Player : public QMainWindow
{
public:
    Player(QWidget *parent = nullptr);
    ~Player();

private:
    QGraphicsScene *scenePlayer;
    int player_id;
    int pos_player_x;
    int pos_player_y;

    void move(int x, int y);
    void validate_candies();
    void takeCandy();


};

#endif // PLAYER_H
