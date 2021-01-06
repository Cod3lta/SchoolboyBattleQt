#include "dataloader.h"
#include <QDebug>

DataLoader::DataLoader()
{
}

void DataLoader::loadCandyAnimations() {
    candiesAnimations.insert(peanut, setupCandyAnimations(-1, 1, 1, "peanut"));
    candiesAnimations.insert(mandarin, setupCandyAnimations(-1, 1, 5, "mandarin"));
}

// CE FICHIER DATALOADER SERA REFAIT SUR UNE NOUVELLE BRANCHE

DataLoader::CandyAnimationsStruct *DataLoader::setupCandyAnimations(int framerate, int nbFrame, int nbPoints, QString filename) {
    CandyAnimationsStruct *c = new CandyAnimationsStruct();
    c->frameIndex = 0;
    c->nbPoints = nbPoints;
    c->image = new QPixmap(":/Resources/candy/" + filename + ".png");
    c->nbFrame = nbFrame;
    // Si on donne un -1 pour le framerate, il n'y a pas d'animation
    if(framerate >= 0) {
        c->timer = new QTimer();
        c->timer->setInterval(framerate);
        c->timer->start();
    }
    qDebug() << "load animation " << filename << " for candies";
    return c;
}
