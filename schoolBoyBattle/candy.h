#ifndef CANDY_H
#define CANDY_H
#include <QGraphicsItem>
#include <QPixmap>
#include "dataloader.h"

class Candy : public QGraphicsItem
{
public:
    Candy(int type, DataLoader *dataLoader, QGraphicsItem *parent = nullptr);
    ~Candy();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;



    enum Type {peanut = 0, mandarin = 1};
    enum Animations {idle = 0};

private:
    typedef struct Animations_s {
        QTimer *timer;
        int frameIndex;
        DataLoader::CandyAnimationsStruct *sharedDatas;
    } AnimationsLocalDatasStruct;

    QHash<Animations, AnimationsLocalDatasStruct*> animations;
    void loadAnimations();
    Candy::AnimationsLocalDatasStruct *setupCandyAnimationData(int framerate, DataLoader::CandyAnimationsStruct *sharedDatas);
    int id;
    Type type;
    DataLoader *dataLoader;
    Animations animationJeTestDautresTrucs;
    void setType(Type t);
    void animationNextFrame();
    void setAnimation(Animations a);
    void setZIndex();
};


#endif // CANDY_H
