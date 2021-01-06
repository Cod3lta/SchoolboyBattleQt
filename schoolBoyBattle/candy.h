#ifndef CANDY_H
#define CANDY_H
#include <QGraphicsItem>
#include <QPixmap>
#include "dataloader.h"

class Candy : public QGraphicsItem, public DataLoader
{
public:
    Candy(int type, QGraphicsItem *parent = nullptr);
    ~Candy();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    typedef struct CandyPlacements_s {
        int x;
        int y;
        bool taken;
        //Candy *candy;
        QTimer *timer;
    } CandyPlacementsStruct;

private:
    int id;
    CandiesType type;
    int points;             // Nombre de points que vaut ce bonbon

};


#endif // CANDY_H
