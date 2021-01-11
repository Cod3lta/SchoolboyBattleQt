#ifndef CANDY_H
#define CANDY_H
#include <QGraphicsItem>
#include <QPixmap>
#include "dataloader.h"

class Candy : public QGraphicsItem
{
public:
    Candy(int candyType, int candySize, DataLoader *dataLoader, QGraphicsItem *parent = nullptr);
    ~Candy();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;



    enum Type : int {peanut = 0, mandarin = 1};
    enum Size : int {small = 0, big = 1};
    enum Animations {idle = 0};

private:
    typedef struct Animations_s {
        QTimer *timer;
        int frameIndex;
        DataLoader::CandyAnimationsStruct *sharedDatas;
    } AnimationsLocalStruct;


    QHash<Animations, AnimationsLocalStruct*> animationsLocal;
    void loadAnimations();
    Candy::AnimationsLocalStruct *setupCandyAnimationData(int framerate, DataLoader::CandyAnimationsStruct *sharedDatas);
    int id;
    Type candyType;
    Size candySize;
    DataLoader *dataLoader;
    Animations animation;
    void setType(Type t);
    void animationNextFrame();
    void setAnimation(Animations a);
    void setZIndex();
};


#endif // CANDY_H
