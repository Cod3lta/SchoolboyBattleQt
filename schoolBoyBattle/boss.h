/*
 * Description : Cette classe s’occupe de créer et d’animer le
 *               Saint-Nicolas et le Père Fouettard qui se trouve
 *               sur le terrain. La classe dérive de Player afin de
 *               bénéficier de son support pour l’animation de l’item.
 * Version     : 1.0.0
 * Date        : 25.01.2021
 * Auteurs     : Prétat Valentin, Badel Kevin et Margueron Yasmine
*/

#include "player.h"

#ifndef BOSS_H
#define BOSS_H

class Boss : public Player
{
public:
    Boss(int team, int x, int y, DataLoader *dataLoader);
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    Boss::AnimationsLocalStruct *setupAnimation(DataLoader::PlayerAnimationsStruct *sharedDatas);
    void loadAnimations();
};

#endif // BOSS_H
