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
    DataLoader(QString terrainFileName, bool isMultiplayer);
    int getTileSize();
    int getPlayerSpeed();
    QVector2D getPlayerSize();
    bool isMultiplayer();
    void setPlayerIndexInMulti(int id);
    int getPlayerIndexInMulti();

private:
    int const tileSize = 130, playerHeight = 150, playerWidth = tileSize, playerSpeed = 8;
    QDomDocument terrainXMLDoc;
    QDomDocument getFileContent(QString fileName);
    bool multiplayer;
    int playerIndexInMulti;         // position du joueur actuel dans la liste "players" si
                                    // on est en multijoueur

    // PLAYER SPAWNPOINTS ----------------------------------------------------------------

public:
    QHash<int, QPoint> teamsSpawnpoints;
    QPoint getTeamSpawnpoint(int teamId);

private:
    void setPlayersSpawnpoint();

    // PLAYER ANIMATIONS -----------------------------------------------------------------

public:
    typedef struct PlayerAnimations_s {
        QPixmap *image;
        int nbFrame;
        int framerate;
    } PlayerAnimationsStruct;
    QHash<int, PlayerAnimationsStruct*> playerAnimations;
    int getPlayerAnimationId(int gender, int team, int animation);
    int getBossAnimationId(int team);

private:
    void loadPlayerAnimations();
    DataLoader::PlayerAnimationsStruct *setupPlayerAnimation(int nbFrame, int framerate, QString fileName);

    // CANDY RESSOURCES ------------------------------------------------------------

public:
    typedef struct CandyRessources_s {
        int nbPoints;
        int candyType;
        int candySize;
        int respawnDelayMs;
    } CandyRessourcesStruct;
    // le int est le tileType (ce qu'il y a dans le .tmx)
    QHash<int, CandyRessourcesStruct*> candiesRessources;
    CandyRessourcesStruct *getCandyRessources(int tileType);

private:
    void loadCandyRessources();
    DataLoader::CandyRessourcesStruct *setupCandyRessources(int nbPoints, int candyType, int candySize, int delayRespawnMs);

    // CANDY ANIMATIONS ------------------------------------------------------------------

public:
    typedef struct CandyAnimations_s {
        QPixmap *image;
        QPixmap *imageBlack;
        QPixmap *imageRed;
        int nbFrame;
        int framerate;
    } CandyAnimationsStruct;
    QHash<int, CandyAnimationsStruct*> candyAnimations;
    int getCandyAnimationId(int type, int size);

private:
    void loadCandyAnimations();
    DataLoader::CandyAnimationsStruct *setupCandyAnimation(int nbFrame, int framerate, QString fileName);

    // TILE LAYERS ----------------------------------------------------------------------

public:
    typedef struct TileLayer_s {
        QList<QList<int>> tiles;
        int width;
        int height;
        int topLeftX;
        int topLeftY;
        int zIndex;
    } TileLayerStruct;
    // doit être dans une qmap car plus tard dans le game.cpp, on prendra chaque layer
    // dans l'ordre pour y construire les tiles (c'est pour ça que les layers sont
    // nommées dans l'ordre alphabétique dans le .tmx)
    QMap<QString, TileLayerStruct*> tileLayers;

private:
    void loadTileLayers();
    QList<QList<int>> setupTileLayer(QDomNodeList chunks);
    void getLayerPlacement(int *layerWidth, int *layerHeight, int *chunkMinX, int *chunkMinY, int size, QDomNodeList chunks);
    QHash<QString, int> highestLowestPointsOfMap();
    void updateTileLayersZIndex();

    // TILES RESSOURCES ------------------------------------------------------------------

public:
    typedef struct TileRessource_s {
        QPixmap *image;
        QString name;   // corresponds au chemin d'accès du fichier depuis :/Resources
    } TileRessourcesStruct;
    // le int est le tileType (ce qu'il y a dans le .tmx)
    QHash<int, TileRessourcesStruct*> tileRessources;
    TileRessourcesStruct* getTileRessource(int tileType);
    int getTileType(QString name);

private:
    void loadTilesRessources();
    QHash<int, QString> loadTilesIds();
};

#endif // DATALOADER_H
