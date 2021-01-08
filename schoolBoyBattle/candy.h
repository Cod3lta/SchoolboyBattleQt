#ifndef CANDY_H
#define CANDY_H
#include <QGraphicsItem>
#include <QPixmap>
#include "dataloader.h"

class Candy : public QGraphicsItem
{
public:
    Candy(int type, QHash<int, DataLoader::CandyAnimationsStruct*> *sharedAnimationsDatas, QGraphicsItem *parent = nullptr);
    ~Candy();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    enum Type {peanut = 0, mandarin = 1};
    typedef struct CandyPlacements_s {
        int x;
        int y;
        bool taken;
        //Candy *candy;
        QTimer *timer;
    } CandyPlacementsStruct;

private:
    typedef struct Animations_s {
        QTimer *timer;
        int frameIndex;
        DataLoader::CandyAnimationsStruct *sharedDatas;
    } AnimationsLocalDatasStruct;
    QHash<Type, AnimationsLocalDatasStruct*> animations;
    void loadAnimations(QHash<int, DataLoader::CandyAnimationsStruct *> *sharedAnimationsDatas);
    Candy::AnimationsLocalDatasStruct *setupCandyAnimationData(int framerate, DataLoader::CandyAnimationsStruct *sharedDatas);
    int id;
    Type type;
};


#endif // CANDY_H
