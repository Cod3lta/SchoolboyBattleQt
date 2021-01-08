/*
 * Classe qui ne sers qu'à contenir des données pour les différents objets
 * de la scène. Toutes les données pour chaque même objet qui sont partagées
 * sont dans cette classe, les données qui diffèrent sont directement dans les
 * classes de ces objets.
 * (tous les players partagent la même sheet d'animation -> la ressource image
 * se trouve ici / tous les joueurs n'affichent pas la même animation en même
 * temps --> la variable frameIndex se trouve dans la classe Player)
 *
 * Un seul objet DataLoader est créé dans le programme.
 */

#ifndef DATALOADER_H
#define DATALOADER_H

#include <QTimer>
#include <QPixmap>

class DataLoader
{
public:
    DataLoader();

    typedef struct PlayerAnimations_s {
        QPixmap *image;
        int nbFrame;
    } PlayerAnimationsStruct;

    typedef struct CandyAnimations_s {
        QPixmap *image;
        int nbFrame;
        int nbPoints;
    } CandyAnimationsStruct;

    // PLAYER ANIMATIONS -----------------------------------------------------------------


private:
    void loadPlayerAnimations();
    DataLoader::PlayerAnimationsStruct *setupPlayerAnimation(int nbFrame, QString fileName);

public:
    QHash<int, DataLoader::PlayerAnimationsStruct*> playerAnimations;
    static int getPlayerAnimationId(int gender, int team, int animation);

    // CANDY ANIMATIONS ------------------------------------------------------------------

private:

    void loadCandyAnimations();
    DataLoader::CandyAnimationsStruct *setupCandyAnimations(int nbFrame, int nbPoints, QString filename);

public:
    QHash<int, CandyAnimationsStruct*> candiesAnimations;
    static int getCandyAnimationId(int type);



    // TILES -----------------------------------------------------------------------------

private:
    int width;
    int height;
    QHash<int, QList<int>> tiles;


    // TILES RESSOURCES ------------------------------------------------------------------

};

#endif // DATALOADER_H
