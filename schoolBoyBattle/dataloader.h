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
    QDomDocument terrainXMLDoc;
    QDomDocument getFileContent(QString fileName);


    // PLAYER ANIMATIONS -----------------------------------------------------------------

public:
    typedef struct PlayerAnimations_s {
        QPixmap *image;
        int nbFrame;
    } PlayerAnimationsStruct;
    QHash<int, PlayerAnimationsStruct*> playerAnimations;
    int getPlayerAnimationId(int gender, int team, int animation);

private:
    void loadPlayerAnimations();
    DataLoader::PlayerAnimationsStruct *setupPlayerAnimation(int nbFrame, QString fileName);

    // CANDY RESSOURCES ------------------------------------------------------------

public:
    typedef struct CandyRessources_s {
        int nbPoints;
        int candyType;
        int candySize;
    } CandyRessourcesStruct;
    QHash<int, CandyRessourcesStruct*> candyRessources;
    CandyRessourcesStruct *getCandyRessources(int type);

private:
    void loadCandyRessources();
    DataLoader::CandyRessourcesStruct *setupCandyRessources(int nbPoints, int candyType, int candySize);

    // CANDY ANIMATIONS ------------------------------------------------------------------

public:
    typedef struct CandyAnimations_s {
        QPixmap *image;
        int nbFrame;
    } CandyAnimationsStruct;
    QHash<int, CandyAnimationsStruct*> candyAnimations;
    int getCandyAnimationId(int type, int size);

private:
    void loadCandyAnimations();
    DataLoader::CandyAnimationsStruct *setupCandyAnimation(int nbFrame, QString fileName);

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
    // doit être dans une qmap car plus tard dans le game.cpp, on prendra chaque layer
    // dans l'ordre pour y construire les tiles (c'est pour ça que les layers sont
    // nommées dans l'ordre alphabétique dans le .tmx)
    QMap<QString, TileLayerStruct*> tileLayers;

private:
    void loadTiles();
    QList<QList<int>> buildLayer(QDomNodeList chunks);
    void getLayerSize(int *layerWidth, int *layerHeight, int size, QDomNodeList chunks);

    // TILES RESSOURCES ------------------------------------------------------------------

public:
    typedef struct TileRessource_s {
        QPixmap *image;
        QString name;   // corresponds au chemin d'accès du fichier depuis :/Resources
    } TileRessourcesStruct;
    QHash<int, TileRessourcesStruct*> tileRessources;
    TileRessourcesStruct* getTileRessource(int type);


private:
    void loadTilesRessources();
    int getTileType(QString name);
    QHash<int, QString> loadTilesIds();

};

#endif // DATALOADER_H
