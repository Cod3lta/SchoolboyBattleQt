#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>

class Tile;
class Niveau;

class Scene : public QGraphicsScene
{
public:
    Scene(QString fichierNiveau, Niveau *parent=0);

private:
    bool prepareScene(QVector < QVector <int> > &tabTextures); // Recupere le tableau contenant le numero des textures
    void placerTextures(QVector < QVector <int> > &tab);

    QString filename;
    Niveau *niveau;
    QList<Tile *> tabMur;
    QList<Tile *> tabFacade;


};

#endif // SCENE_H
