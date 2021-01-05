#include "niveau.h"
#include "scene.h"

Niveau::Niveau(QString fichierNiveau, QWidget *parent) : QGraphicsView(parent)
{
    scene = new Scene(fichierNiveau, this);
    setScene(scene);
}
