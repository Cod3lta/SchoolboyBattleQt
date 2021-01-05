#ifndef DOMPARSER_H
#define DOMPARSER_H

#include <vector>
#include <QVector>
#include <QVector2D>

class QFile;
class QTreeWidget;
class QTreeWidgetItem;
//class QVector;
class QVector2D;
class QDomDocument;
class QDomElement;

class DOMParser
{
public:
    DOMParser(QFile *fichier, QTreeWidget *t);
    QVector< QVector<int> > getDonnees();
    int getHauteur();
    int getLongeur();

private:
    QTreeWidget *tree;
    QTreeWidgetItem *racine;

    int longueur;
    int hauteur;

    QVector<QVector <int> > tabDonnees;
};

#endif // DOMPARSER_H
