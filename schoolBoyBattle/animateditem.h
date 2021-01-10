#ifndef ANIMATEDITEM_H
#define ANIMATEDITEM_H

#include "dataloader.h"

#include <QTimer>
#include <QHash>



class AnimatedItem
{
public:
    AnimatedItem();

protected:
    typedef struct AnimationsLocal_s {
        QTimer *timer;
        int frameIndex;
    } AnimationsLocalStruct;
    QHash<int, AnimationsLocalStruct*> animationsLocal;

    //void loadAnimations();
    //AnimationsLocalStruct* setupAnimation(int framerate, DataLoader dataLoader);

};

#endif // ANIMATEDITEM_H
