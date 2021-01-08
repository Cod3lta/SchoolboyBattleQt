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
#include <QDomDocument>

class DataLoader
{
public:
    DataLoader(QString terrainFileName);



    // PLAYER ANIMATIONS -----------------------------------------------------------------

public:
    typedef struct PlayerAnimations_s {
        QPixmap *image;
        int nbFrame;
    } PlayerAnimationsStruct;
    QHash<int, DataLoader::PlayerAnimationsStruct*> playerAnimations;
    static int getPlayerAnimationId(int gender, int team, int animation);

private:
    void loadPlayerAnimations();
    DataLoader::PlayerAnimationsStruct *setupPlayerAnimation(int nbFrame, QString fileName);

    // CANDY ANIMATIONS ------------------------------------------------------------------


public:
    typedef struct CandyAnimations_s {
        QPixmap *image;
        int nbFrame;
        int nbPoints;
    } CandyAnimationsStruct;
    QHash<int, CandyAnimationsStruct*> candiesAnimations;
    static int getCandyAnimationId(int type);

private:
    void loadCandyAnimations();
    DataLoader::CandyAnimationsStruct *setupCandyAnimations(int nbFrame, int nbPoints, QString filename);

    // TILES -----------------------------------------------------------------------------

public:
    typedef struct TileLayer_s {
        QList<QList<int>> tiles;
        int width;
        int height;
        int topLeftX;
        int topLeftY;
    } TileLayerStruct;
    QList<TileLayerStruct*> tileLayers;

private:
    void loadTiles(QString terrainFileName);
    QList<QList<int>> buildLayer(QDomNodeList chunks);
    void getLayerSize(int *layerWidth, int *layerHeight, int size, int firstChunkY, QDomNodeList chunks);

    // TILES RESSOURCES ------------------------------------------------------------------

public:
    QHash<int, QPixmap*> tileRessources;
    static QPixmap* getTileRessource(int id);


private:
    void loadTilesRessources();

};

#endif // DATALOADER_H
