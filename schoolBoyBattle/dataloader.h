/*
 * Classe qui ne sers qu'à contenir des données pour les différents objets
 * de la scène. Toutes les données pour chaque même objet qui sont partagées
 * sont dans cette classe, les données qui diffèrent sont directement dans les
 * classes de ces objets.
 * (tous les players partagent la même sheet d'animation -> la ressource image
 * se trouve ici / tous les joueurs n'affichent pas la même animation en même
 * temps --> la variable frameIndex se trouve dans la classe Player)
 */

#ifndef DATALOADER_H
#define DATALOADER_H

#include <QTimer>
#include <QPixmap>

class DataLoader
{
public:
    DataLoader();

    typedef struct Animations_s {
        QPixmap *image;
        //QTimer *timer;
        int nbFrame;
        //int frameIndex;
    } AnimationsStruct;

    // PLAYER ----------------------------------------------------------------------------


private:

public:
    QHash<int, DataLoader::AnimationsStruct*> playerAnimations;
    QHash<int, AnimationsStruct*> *getPlayerAnimations();
    void loadPlayerAnimations();
    DataLoader::AnimationsStruct *setupPlayerAnimation(int nbFrame, QString fileName);
    static int getPlayerAnimationId(int gender, int team, int animation);

    // CANDY -----------------------------------------------------------------------------
    /*
public:
    void loadCandyAnimations();
    typedef struct CandyAnimationDatas_s {
        QTimer *timer;
        QPixmap* image;
        int nbFrame;
        int frameIndex;
        int nbPoints;
    } CandyAnimationsStruct;
    enum CandiesType { peanut = 0, mandarin = 1};

private:

    QHash<CandiesType, CandyAnimationsStruct*> candiesAnimations;

private:
    CandyAnimationsStruct *setupCandyAnimations(int framerate, int nbFrame, int nbPoints, QString filename);
    */


    // TILE ------------------------------------------------------------------------------


};

#endif // DATALOADER_H
