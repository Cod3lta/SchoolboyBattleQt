#ifndef PLAYER_H
#define PLAYER_H

#include <QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>



class Player : public QGraphicsItem
{
public:
    Player(QGraphicsItem *parent = nullptr);
    ~Player();

    //virtual
    //void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) = 0;
    //QRectF boundingRect() const = 0;
    //QPainterPath shape() const = 0;

public:
    QRectF boundingRect() const
    {
        qreal penWidth = 1;
        return QRectF(-10 - penWidth / 2, -10 - penWidth / 2,
                      20 + penWidth, 20 + penWidth);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget)
    {
        painter->drawRoundedRect(-10, -10, 20, 20, 5, 5);
    }

    QPainterPath shape() const
    {
        //QPainterPath ellipsePath;
        //ellipsePath.moveTo(80.0, 50.0);
        //ellipsePath.arcTo(20.0, 30.0, 60.0, 40.0, 0.0, 360.0);
        //return ellipsePath;

        QPainterPath path;
        path.addEllipse(boundingRect());
        return path;
    }


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

protected:
    void keyReleaseEvent(QKeyEvent *e);


};

#endif // PLAYER_H
