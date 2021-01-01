#ifndef PLAYER_H
#define PLAYER_H

#include <QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>



class Player : public QGraphicsObject
{
public:
    Player(int id, int team, QGraphicsObject *parent = nullptr);
    ~Player();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void move();

private:
    enum Teams : int {red = 0, black = 1};
    enum Gender : int {girl = 0, boy = 1};
    enum PlayerMovesEnum : int {up = 0, right = 1, down = 2, left = 3};

    int id;
    Teams team;
    Gender gender;
    bool moves[4] = {false, false, false, false};

    QHash<Teams, QList<int>> teamsSpawnpoint;
    QGraphicsRectItem *debugRect;
    QPixmap *animationIdle;
    QPixmap *animationRun;
    QTimer *timerAnimationIdle;
    QTimer *timerAnimationRun;

    void validate_candies();
    void takeCandy();

public slots:
    void keyMove(int playerId, int direction, bool value);

};

#endif // PLAYER_H
