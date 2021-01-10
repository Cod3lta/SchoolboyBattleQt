#ifndef ANIMATEDITEM_H
#define ANIMATEDITEM_H

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
};

#endif // ANIMATEDITEM_H
