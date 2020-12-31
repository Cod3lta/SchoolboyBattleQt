#ifndef PLAYER_H
#define PLAYER_H

#include <QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>



class Player : public QGraphicsObject
{
public:
    Player(int id, int x, int y, QGraphicsObject *parent = nullptr);
    ~Player();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void move();

private:
    int id;
    bool moves[4] = {false, false, false, false};
    enum PlayerMovesEnum : int {up = 0, right = 1, down = 2, left = 3};

    QGraphicsRectItem *debugRect;
    void validate_candies();
    void takeCandy();
    QPixmap *animationIdle;
    QPixmap *animationRun;
    QTimer *timerAnimationIdle;
    QTimer *timerAnimationRun;
    enum Teams : int {red = 0, black = 1};

public slots:
    void keyMove(int playerId, int direction, bool value);

};

#endif // PLAYER_H
