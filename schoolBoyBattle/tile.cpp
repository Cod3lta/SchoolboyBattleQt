#include "tile.h"
#include "domparser.h"

#include <QPainter>
#include <QHash>
#include <QTreeWidget>
#include <QFile>
#include <QDebug>

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

QHash<Tile::Type, Tile::TileDataStruct*> Tile::loadTilesData() {
    QHash<Tile::Type, TileDataStruct*> tilesData;
    tilesData.insert(mur, setupTileData(true, "mur.png"));
    tilesData.insert(sol, setupTileData(false, "sol.png"));
    tilesData.insert(facade, setupTileData(true, "facade.png"));
    return tilesData;
}

QHash<Tile::Type, Tile::TileDataStruct*> Tile::tilesData = Tile::loadTilesData();

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


bool Tile::prepareScene(QFile *filename)
{
    //QFile *file = new QFile(filename);

    QTreeWidget *tree = new QTreeWidget();
    DOMParser parser(filename, tree);

    // Recuperer le numero des textures
    QVector< QVector <int> > tabTextures = parser.getDonnees();

    // Definir la taille de la scene
    //setSceneRect(0, 0, parser.getLongeur() * 25, parser.getHauteur() * 25);

    qDebug() << "w : " << parser.getLongeur() << " - h : " << parser.getHauteur();

    // Suppression des pointeurs dont on n'a plus besoin
    delete tree;

    // Placer les textures dans la scene
    placeTexture(tabTextures);

    // TODO: tester si on a bien reussi a obtenir les textures du fichier (pas que le fichier soit corrompu)
    return true;


}


void Tile::placeTexture(QVector < QVector <int> > &tab)
{
    const int X = 0;
    const int Y = 0;

    // Placer et enregistrer les textures dans tabBackground
    // Initialisation des variables pour positionner les textures
    int posX = X;
    int posY = Y;

    // Variable pour l'affichage des textures (considerees comme item)
    //QPixmap pixmap;
    //Tile *tile; // TODO: a optimiser car ne detruit pas les objets
    QGraphicsItem *item;

    // Parcours du tableau pour l'affichage des textures (background du niveau)
    for (int i = 0; i < tab.size(); ++i)
    {
        for (int j = 0; j < tab[i].size(); ++j)
        {
            switch (tab[i][j])
            {
            case 2:
                item = new Tile(Tile::mur, posX, posY, 0);
                break;
            case 3:
                posY += 19; // La hauteur de l'image est plus petite (25x6)
                item = new Tile(Tile::sol, posX, posY, 1);
                posY -= 19;
                break;
            case 4:
                item = new Tile(Tile::facade, posX, posY, 2);
                break;
            default:
                item = NULL;
                break;
            }

            // Si c'est un mur ou une facade, on l'ajoute
            if (tab[i][j] == 2)
            {
                tabMur.append((Tile *) item);
            }
            else if (tab[i][j] == 4)
            {
                tabFacade.append((Tile *) item);
            }

            if (item != NULL)
            {
                //addItem(item);
            }

            posX += 25;
        }
        posX = X;
        posY += 25;
    }
}



Tile::~Tile() {

}
