#ifndef BOSS_H
#define BOSS_H

#include "player.h"

class Boss : public Player
{
public:
    Boss(int team, int x, int y, DataLoader *dataLoader);
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

private:

    Boss::AnimationsLocalStruct *setupAnimation(DataLoader::PlayerAnimationsStruct *sharedDatas);
    void loadAnimations();
};

#endif // BOSS_H
