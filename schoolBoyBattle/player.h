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
    //QGraphicsScene *scenePlayer;
    int player_id;
    int pos_player1_x;
    int pos_player1_y;
    int pos_player2_x;
    int pos_player2_y;
    QGraphicsEllipseItem *ellipse;
    QGraphicsRectItem *rectangle;

    //void move(int x, int y);
    void validate_candies();
    void takeCandy();

    void keyReleaseEvent(QKeyEvent *e);


};

#endif // PLAYER_H
