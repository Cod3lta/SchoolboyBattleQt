#ifndef DATALOADER_H
#define DATALOADER_H

#include <QTimer>
#include <QPixmap>

// CE FICHIER DATALOADER SERA REFAIT SUR UNE NOUVELLE BRANCHE

class DataLoader
{
public:
    DataLoader();


    // CANDY -----------------------------------------------------------------------------
public:
    void loadCandyAnimations();

protected:
    typedef struct CandyAnimationDatas_s {
        QTimer *timer;
        QPixmap* image;
        int nbFrame;
        int frameIndex;
        int nbPoints;
    } CandyAnimationsStruct;
    enum CandiesType { peanut = 0, mandarin = 1};
    QHash<CandiesType, CandyAnimationsStruct*> candiesAnimations;

private:
    CandyAnimationsStruct *setupCandyAnimations(int framerate, int nbFrame, int nbPoints, QString filename);

    // PLAYER ----------------------------------------------------------------------------


    // TILE ------------------------------------------------------------------------------


};

#endif // DATALOADER_H
