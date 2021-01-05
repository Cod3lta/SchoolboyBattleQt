#include "candy.h"
#include <QBrush>
#include <QPen>
#include "game.h"

Candy::Candy(int type, QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    this->type = static_cast<Type>(type);
}

// Setup des animations des bonbons

QHash<Candy::Type, Candy::CandyDatasStruct*> Candy::loadCandyDatas() {
    QHash<Candy::Type, Candy::CandyDatasStruct*> candiesData;
    candiesData.insert(bleu, setupCandyData(-1, 1, 1, "bleu"));
    candiesData.insert(rouge, setupCandyData(-1, 1, 1, "rouge"));
    candiesData.insert(vert, setupCandyData(-1, 1, 1, "vert"));
    candiesData.insert(jaune, setupCandyData(-1, 1, 1, "jaune"));
    candiesData.insert(orange, setupCandyData(-1, 1, 1, "orange"));
    candiesData.insert(rose, setupCandyData(-1, 1, 1, "rose"));
    return candiesData;
}

Candy::CandyDatasStruct* Candy::setupCandyData(int framerate, int nbFrames, int nbPoints, QString filename) {
    CandyDatasStruct *c = new CandyDatasStruct();
    c->frameIndex = 0;
    c->nbPoints = nbPoints;
    // TODO : à décommenter quand les ressources seront crées
    //c->image = new QPixmap(":/Resources/candy/" + filename + ".png");
    c->nbFrames = nbFrames;
    // Si on donne un -1 pour le framerate, il n'y a pas d'animation
    if(framerate >= 0) {
        c->timer = new QTimer();
        c->timer->setInterval(framerate);
        c->timer->start();
    }
    return c;
}

QHash<Candy::Type, Candy::CandyDatasStruct*> candiesDatas = Candy::loadCandyDatas();
