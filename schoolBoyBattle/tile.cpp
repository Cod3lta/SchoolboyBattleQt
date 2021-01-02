#include "tile.h"
#include <QPainter>
#include <QHash>

//QHash<Tile::Type, QString> *Tile::tilesData;
//Tile::tilesData.insert(mur, "mur");
//tilesData.insert(sol, "sol");
//tilesData.insert(facade, "facade");

Tile::Tile(Type type, int x, int y, int layer, QGraphicsObject *parent)
    : QGraphicsItem(parent),
      layer(layer),
      type(type) {
    setPos(x, y);
}

void Tile::loadTilesData() {
    tilesData.insert(mur, setupTileData(true, "mur.png"));
    tilesData.insert(sol, setupTileData(false, "sol.png"));
    tilesData.insert(facade, setupTileData(true, "facade.png"));
}

Tile::TileDataStruct* Tile::setupTileData(bool collision, QString filename) {
    TileDataStruct *t = new TileDataStruct();
    t->collision = collision;
    t->image = new QPixmap(":/Resources/img/tile/" + filename);
    return t;
}

QPixmap* Tile::getTextureByType() {
    QPixmap *p = new QPixmap();

    return p;
}

// OVERRIDE REQUIRED

// Paints contents of item in local coordinates
void Tile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setPen(QPen(Qt::black));
    painter->setBrush(QBrush(Qt::red));
    painter->drawRect(pos().x(), pos().y(), 50, 50);
    painter->drawText(pos().x()+10, pos().y()+10, QString::number(x()));
    // Lignes pour le compilateur
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

// Returns outer bounds of item as a rectangle
// Called by QGraphicsView to determine what regions need to be redrawn
QRectF Tile::boundingRect() const {
    return QRectF(pos().x(), pos().y(), 100, 100);
}

// collisions detection
QPainterPath Tile::shape() const {

    //QPainterPath ellipsePath;
    //ellipsePath.moveTo(80.0, 50.0);
    //ellipsePath.arcTo(20.0, 30.0, 60.0, 40.0, 0.0, 360.0);
    //return ellipsePath;


    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}


Tile::~Tile() {

}
