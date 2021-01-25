/*
 * Description : Cette classe s’occupe de créer et d’animer le
 *               Saint-Nicolas et le Père Fouettard qui se trouve
 *               sur le terrain. La classe dérive de Player afin de
 *               bénéficier de son support pour l’animation de l’item.
 * Version     : 1.0.0
 * Date        : 25.01.2021
 * Auteurs     : Prétat Valentin, Badel Kevin et Margueron Yasmine
*/

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

void Boss::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    AnimationsLocalStruct *animToDraw = animationsLocal.value(idle);
    QPixmap *imageToDraw = animToDraw->sharedDatas->image;

    QRectF sourceRect = QRectF(imageToDraw->width() / animToDraw->sharedDatas->nbFrame * animToDraw->frameIndex, 0,
                               imageToDraw->width() / animToDraw->sharedDatas->nbFrame, imageToDraw->height());
    QRectF targetRect = QRectF(boundingRect());
    painter->drawPixmap(targetRect, *imageToDraw, sourceRect);

    // Lignes pour le compilateur
    Q_UNUSED(option)
    Q_UNUSED(widget)
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
