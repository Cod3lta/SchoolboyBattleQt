#include "boss.h"

#define BOSS_WIDTH 260
#define BOSS_HEIGHT 390

Boss::Boss(int team, int x, int y, DataLoader *dataLoader) :
    Player(team, dataLoader)
{
    setPos(x , y - BOSS_HEIGHT * 2/ 3);
    loadAnimations();
    setAnimation(idle);
    facing = facingRight;
    setZIndex(BOSS_HEIGHT);
}

QRectF Boss::boundingRect() const {
    return QRectF(0, 0, BOSS_WIDTH, BOSS_HEIGHT);
}

// collisions detection
QPainterPath Boss::shape() const {
    QPainterPath path;
    path.addRect(QRectF(0, 0, 0, 0));
    return path;
}

void Boss::loadAnimations() {
    animationsLocal.insert(idle, setupAnimation(dataLoader->playerAnimations.value(dataLoader->getBossAnimationId(team))));
}

Boss::AnimationsLocalStruct* Boss::setupAnimation(DataLoader::PlayerAnimationsStruct* sharedDatas) {
    AnimationsLocalStruct* aStruct = new AnimationsLocalStruct;
    aStruct->frameIndex = 0;
    aStruct->timer = new QTimer();
    aStruct->timer->setInterval(sharedDatas->framerate);
    aStruct->timer->stop();
    connect(aStruct->timer, &QTimer::timeout, this, &Boss::animationNextFrame);
    aStruct->sharedDatas = sharedDatas;
    return aStruct;
}
