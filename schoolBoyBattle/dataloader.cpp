#include "dataloader.h"
#include <QDebug>
#include <QtXml>
#include <QFile>
#include <QHash>
#include <QDomDocument>

DataLoader::DataLoader(QString terrainFileName)
{
    loadPlayerAnimations();
    loadCandyAnimations();
    terrainXMLDoc = getFileContent(terrainFileName);
    loadTiles();
    loadTilesRessources();
}

QDomDocument DataLoader::getFileContent(QString fileName) {
    QDomDocument xmlBOM;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        qFatal("Erreur de lecture du fichier");
    xmlBOM.setContent(&file);
    file.close();
    return xmlBOM;
}

// PLAYER -----------------------------------------------------------------------------------

void DataLoader::loadPlayerAnimations() {
    playerAnimations.insert(0, setupPlayerAnimation(6, ":/Resources/player/idle/boy-black-idle.png"));
    playerAnimations.insert(1, setupPlayerAnimation(6, ":/Resources/player/idle/girl-black-idle.png"));
    playerAnimations.insert(2, setupPlayerAnimation(6, ":/Resources/player/idle/boy-red-idle.png"));
    playerAnimations.insert(3, setupPlayerAnimation(6, ":/Resources/player/idle/girl-red-idle.png"));
    playerAnimations.insert(4, setupPlayerAnimation(10, ":/Resources/player/run/boy-black-run.png"));
    playerAnimations.insert(5, setupPlayerAnimation(10, ":/Resources/player/run/girl-black-run.png"));
    playerAnimations.insert(6, setupPlayerAnimation(10, ":/Resources/player/run/boy-red-run.png"));
    playerAnimations.insert(7, setupPlayerAnimation(10, ":/Resources/player/run/girl-red-run.png"));
}

DataLoader::PlayerAnimationsStruct* DataLoader::setupPlayerAnimation(int nbFrame, QString fileName) {
    PlayerAnimationsStruct* aStruct = new PlayerAnimationsStruct;
    aStruct->nbFrame = nbFrame;
    aStruct->image = new QPixmap(fileName);
    return aStruct;
}

int DataLoader::getPlayerAnimationId(int gender, int team, int animation) {
    // TODO : Améliorer ce code un peu moche
    if(gender == 0 && team == 0 && animation == 0) return 3;
    if(gender == 1 && team == 0 && animation == 0) return 2;
    if(gender == 0 && team == 1 && animation == 0) return 1;
    if(gender == 1 && team == 1 && animation == 0) return 0;
    if(gender == 0 && team == 0 && animation == 1) return 7;
    if(gender == 1 && team == 0 && animation == 1) return 6;
    if(gender == 0 && team == 1 && animation == 1) return 5;
    if(gender == 1 && team == 1 && animation == 1) return 4;
    return -1;
}

// CANDY ------------------------------------------------------------------------------------

void DataLoader::loadCandyAnimations() {
    candiesAnimations.insert(0, setupCandyAnimations(1, 1, ":/Resources/candy/peanut-small.png"));
    candiesAnimations.insert(1, setupCandyAnimations(1, 5, ":/Resources/candy/mandarin-small.png"));
}

DataLoader::CandyAnimationsStruct * DataLoader::setupCandyAnimations(int nbFrame, int nbPoints, QString filename) {
    CandyAnimationsStruct *c = new CandyAnimationsStruct();
    c->nbPoints = nbPoints;
    c->image = new QPixmap(filename);
    c->nbFrame = nbFrame;
    return c;

}

int DataLoader::getCandyAnimationId(int type) {
    // TODO : Améliorer ce code un peu moche
    if(type == 0) return 0;
    if(type == 1) return 1;
    return -1;
}

// TILE -------------------------------------------------------------------------------------

// https://lucidar.me/fr/dev-c-cpp/reading-xml-files-with-qt/
void DataLoader::loadTiles() {

    // Lire le fichier XML et créer les ressources nécessaires
    QDomNodeList layers = terrainXMLDoc.elementsByTagName("layer");
    for(int i = 0; i < layers.count(); i++) {
        QDomElement layer = layers.at(i).toElement();
        // Chaque layer
        qDebug() << layer.attribute("name");

        TileLayerStruct *tileLayer = new TileLayerStruct();

        // Prendre chaque chunk de la layer
        QDomNodeList chunks = layer.firstChild().childNodes();
        tileLayer->tiles = buildLayer(chunks);

        tileLayer->height = tileLayer->tiles.size();
        tileLayer->width = tileLayer->tiles.at(0).size();
        QDomElement firstChunk = layers.at(i).firstChild().firstChildElement();
        tileLayer->topLeftX = firstChunk.attributes().namedItem("x").nodeValue().toInt();
        tileLayer->topLeftY = firstChunk.attributes().namedItem("y").nodeValue().toInt();

        tileLayers.append(tileLayer);
    }
}

QList<QList<int>> DataLoader::buildLayer(QDomNodeList chunks) {

    QList<QList<int>> dimLevel;

    int firstChunkX = chunks.at(0).toElement().attribute("x").toInt();
    int firstChunkY = chunks.at(0).toElement().attribute("y").toInt();
    int size = chunks.at(0).toElement().attribute("width").toInt();
    int layerWidth = 0;
    int layerHeight = 0;

    // Déterminer la taille de la layer
    getLayerSize(&layerWidth, &layerHeight, size, firstChunkY, chunks);

    // Initialiser la liste
    for(int i = 0; i < layerHeight; i++) {
        QList<int> subList;
        for(int j = 0; j < layerWidth; j++) {
            subList.append(0);
        }
        dimLevel.append(subList);
    }

    for(int i = 0; i < chunks.length(); i++) {
        QDomElement chunk = chunks.at(i).toElement();
        if(chunk.attribute("width") != chunk.attribute("height"))
            qFatal("Un chunk n'est pas carré");

        QList<QString> stringList = chunk.text().replace("\n", "").replace("\r", "").split(",");
        QList<int> intList;
        for(int i = 0; i < stringList.length(); i++) {
            intList.append(stringList.at(i).toInt());
        }
        for(int j = 0; j < size; j++) {
            for(int k = 0; k < size; k++) {
                int insertYList = chunk.attribute("y").toInt() + j - firstChunkY;
                int insertXList = chunk.attribute("x").toInt() + k - firstChunkX;
                QList<int> subList = dimLevel.value(insertYList);
                subList.replace(insertXList, intList.at(j*size + k));
                dimLevel.replace(insertYList, subList);
            }
        }
    }
    return dimLevel;
}

/*
 * Mets dans les variables layerWidth et layerHeight la taille de la layer
 * (nombre de tiles en x et nombre de tiles en y)
 */
void DataLoader::getLayerSize(int *layerWidth, int *layerHeight, int size, int firstChunkY, QDomNodeList chunks) {
    if(chunks.length() == 1) {
        *layerWidth = size;
        *layerHeight = size;
        return;
    }

    int currentY = firstChunkY;
    for(int i = 0; i < chunks.length(); i++) {
        QDomElement chunk = chunks.at(i).toElement();
        if(currentY < chunk.attribute("y").toInt()) {
            *layerWidth = i*size;
            break;
        }
        currentY = chunk.attribute("y").toInt();
    }
    *layerHeight = chunks.length() / (*layerWidth/size) * size;

}

// TILE RESSOURCES --------------------------------------------------------------------------

void DataLoader::loadTilesRessources() {
    QHash<int, QString> tilesIds = loadTilesIds();
    QHashIterator<int, QString> tilesIdsIterator(tilesIds);
    while (tilesIdsIterator.hasNext()) {
        tilesIdsIterator.next();
        tileRessources.insert(
                    tilesIdsIterator.key(),
                    new QPixmap(":/Resources/world/" + tilesIdsIterator.value()));
    }
}

QHash<int, QString> DataLoader::loadTilesIds() {
    QHash<int, QString> tilesIds;
    QDomNodeList tilesets = terrainXMLDoc.elementsByTagName("tileset");
    for(int i = 0; i < tilesets.count(); i++) {
        QDomElement tileset = tilesets.at(i).toElement();
        qDebug() << tileset.attribute("name");
        int tilesetId = tileset.attribute("firstgid").toInt();
        QDomNodeList tiles = tileset.childNodes();
        for(int j = 0; j < tiles.size(); j++) {
            QDomElement tile = tiles.at(j).toElement();
            qDebug() << tile.tagName();
            if(tile.tagName() == "tile") {
                int tileId = tile.attribute("id").toInt();
                QString tileFile = tile.firstChild().toElement().attribute("source");
                tilesIds.insert(tilesetId + tileId, tileFile);
            }
        }
    }
    return tilesIds;
}

QPixmap* DataLoader::getTileRessource(int type) {
    return tileRessources.value(type);
}
