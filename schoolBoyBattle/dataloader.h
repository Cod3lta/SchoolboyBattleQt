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

#include <QMap>
#include <QTimer>
#include <QPixmap>
#include <QDomDocument>

class DataLoader
{
public:
    DataLoader(QString terrainFileName);

private:
    QDomDocument getFileContent(QString fileName);


    // PLAYER ANIMATIONS -----------------------------------------------------------------

public:
    typedef struct PlayerAnimations_s {
        QPixmap *image;
        int nbFrame;
    } PlayerAnimationsStruct;
    QHash<int, DataLoader::PlayerAnimationsStruct*> playerAnimations;
    int getPlayerAnimationId(int gender, int team, int animation);

private:
    QDomDocument terrainXMLDoc;
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
    CandyAnimationsStruct *setupCandyAnimations(int nbFrame, int nbPoints, QString filename);

    // CANDY PLACEMENTS ------------------------------------------------------------------

public:
    typedef struct CandyPlacements_s {
        int x;
        int y;
        bool taken;
        //Candy *candy;
        QTimer *timer;
    } CandyPlacementStruct;
    QList<CandyPlacementStruct*> candyPlacements;

private:
    void loadCandyPlacements();
    CandyPlacementStruct *setupCandyPlacement(int x, int y, QString name);



    // TILE LAYERS ----------------------------------------------------------------------

public:
    typedef struct TileLayer_s {
        QList<QList<int>> tiles;
        int width;
        int height;
        int topLeftX;
        int topLeftY;
    } TileLayerStruct;
    QMap<QString, TileLayerStruct*> tileLayers;

private:
    void loadTiles();
    QList<QList<int>> buildLayer(QDomNodeList chunks);
    void getLayerSize(int *layerWidth, int *layerHeight, int size, QDomNodeList chunks);

    // TILES RESSOURCES ------------------------------------------------------------------

public:
    typedef struct TileRessource_s {
        QPixmap *image;
        QString name;
    } TileRessourceStruct;
    QHash<int, TileRessourceStruct*> tileRessources;
    TileRessourceStruct* getTileRessource(int type);


private:
    void loadTilesRessources();
    QHash<int, QString> loadTilesIds();

};

#endif // DATALOADER_H
