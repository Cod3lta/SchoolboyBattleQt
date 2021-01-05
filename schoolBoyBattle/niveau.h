#ifndef NIVEAU_H
#define NIVEAU_H

#include <QGraphicsView>
class Scene;

class Niveau : public QGraphicsView
{
public:
    Niveau(QString filenameNiveau, QWidget *parent=0);

private:
    Scene *scene;
};

#endif // NIVEAU_H
