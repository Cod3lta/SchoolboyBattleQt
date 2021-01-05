#include "scene.h"
#include "domparser.h"
#include "niveau.h"
#include "tile.h"

#include <QFile>
#include <QTreeWidget>
#include <QDebug>

Scene::Scene(QString fichierNiveau, Niveau *parent)
{
    niveau = parent;
    filename = fichierNiveau;

    QVector < QVector <int> > tabTextures; // Variable locale contenant le numero des textures

    // On prepare la scene
    prepareScene(tabTextures);
}

bool Scene::prepareScene(QVector< QVector <int> > &tabTextures)
{
    QFile *file = new QFile(filename);

    QTreeWidget *tree = new QTreeWidget();
    DOMParser parser(file, tree);

    // Recuperer le numero des textures
    tabTextures = parser.getDonnees();

    // Definir la taille de la scene
    setSceneRect(0, 0, parser.getLongeur() * 25, parser.getHauteur() * 25);

    qDebug() << "w : " << parser.getLongeur() << " - h : " << parser.getHauteur();

    // Suppression des pointeurs dont on n'a plus besoin
    delete tree;
    delete file;

    // Placer les textures dans la scene
    placerTextures(tabTextures);

    // Ajouter Mario
    //mario = new Mario();
    //addItem(mario);

    // TODO: tester si on a bien reussi a obtenir les textures du fichier (pas que le fichier soit corrompu)
    return true;


}


void Scene::placerTextures(QVector < QVector <int> > &tab)
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

            // Si c'est une pique, on l'ajoute
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
                addItem(item);
            }

            posX += 25;
        }
        posX = X;
        posY += 25;
    }
}
