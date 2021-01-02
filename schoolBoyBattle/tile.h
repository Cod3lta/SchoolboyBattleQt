/*
 *
 */

#ifndef TILE_H
#define TILE_H

#include <QGraphicsItem>

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
    static void loadTilesData();
    static TileDataStruct* setupTileData(bool collision, QString filename);


    Tile(Type type, int x, int y, int layer, QGraphicsObject *parent = nullptr);
    ~Tile();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QString *filename;   //
    bool *collision;     //
    int layer;
    QPixmap *img;        //
    Type type;

    QPixmap *getTextureByType();
    void setupTiles();
};

#endif // TILE_H
