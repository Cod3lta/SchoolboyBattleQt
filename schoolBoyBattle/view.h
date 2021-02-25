/*
 * Description : Cette classe dérive de QGraphicsView et crée la ou les vues
 *               qui permettront de visualiser la scène.
 * Version     : 1.0.0
 * Date        : 25.01.2021
 * Auteurs     : Prétat Valentin, Badel Kevin et Margueron Yasmine
*/

#include <QGraphicsView>
#include "game.h"

#ifndef VIEW_H
#define VIEW_H

class Game;

class View : public QGraphicsView
{
    Q_OBJECT

public:
    View(int id, QGraphicsView *parent = nullptr);
    void moveView(Player *player, int playerWidth, int playerHeight, double deltaMs);

private:
    int id;
    float x;
    float y;
};

#endif // VIEW_H
