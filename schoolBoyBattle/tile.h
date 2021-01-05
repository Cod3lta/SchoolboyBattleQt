/*
 *
 */

#ifndef TILE_H
#define TILE_H

#include <QGraphicsItem>
#include <QString>
#include <QFile>

class Tile : public QGraphicsItem
{
public:
    enum Type {mur, sol, facade};

    // Tableau de struct qui contient toutes les données de chaque tile
    typedef struct tileData_s {
        QPixmap *image;
        bool collision;
    }TileDataStruct;
    static QHash<Type, TileDataStruct*> tilesData;
    static QHash<Type, TileDataStruct*> loadTilesData();
    static TileDataStruct* setupTileData(bool collision, QString filename);


    Tile(Type type, int x, int y, int layer, QGraphicsObject *parent = nullptr);
    ~Tile();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    bool prepareScene(QFile *filename);



private:
    QString *filename;
    bool *collision;
    int layer;
    QPixmap *img;
    Type type;

    QList<Tile *> tabMur;
    QList<Tile *> tabFacade;

    QPixmap *getTextureByType();
    void setupTiles();
    void placeTexture(QVector < QVector <int> > &tab);


};

#endif // TILE_H
