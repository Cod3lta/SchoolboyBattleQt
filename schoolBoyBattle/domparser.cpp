#include "domparser.h"

#include <QStringList>
#include <QtGui>
#include <QTreeWidgetItem>
#include <QDomDocument>
#include <QDomElement>


class QDomDocument;
class QDomElement;

DOMParser::DOMParser(QFile *fichier, QTreeWidget *t)
{
    bool error = false;

    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument doc;
    if (!doc.setContent(fichier, true, &errorStr, &errorLine, &errorColumn))
    {
        error = true;
    }
    else
    {
        QDomElement root = doc.documentElement(); // <map>

        hauteur = root.attribute("height").toInt();
        longueur = root.attribute("width").toInt();

        tree = t;
        racine = new QTreeWidgetItem(tree);
        racine->setText(0, root.tagName());

        // Recuperer le premier fils de la racine
        QDomElement node = root.firstChildElement(); // <tileset>, pas important
        node = node.nextSiblingElement(); // <layer>, pas important
        node = node.firstChildElement(); // <data>

        // Verifier s'il s'agit de donnees CSV
        if (node.attribute("encoding") != "csv")
        {
            error = true;
        }
        else
        {
            // Recuperer les donnees
            QString data = node.text();
            if (data.startsWith("\n"))
            {
                data = data.remove(0, 1);
            }
            if (data.endsWith("\n"))
            {
                data = data.remove(data.length() - 1, 1);
            }
            // Separer les lignes
            QStringList lignes = data.split("\n");

            tabDonnees.resize(lignes.count());

            // Separer les colonnes
            for (int i = 0; i < lignes.count(); i++)
            {
                QStringList colonnes = lignes.at(i).split(",");
                if (colonnes.at(colonnes.count() - 1) == "")
                {
                    colonnes.removeAt(colonnes.count() - 1);
                }
                tabDonnees[i].resize(colonnes.count());
                for (int j = 0; j < colonnes.count(); j++)
                {
                    tabDonnees[i][j] = colonnes.at(j).toInt();
                }
            }
        }
    }
}


QVector < QVector <int> > DOMParser::getDonnees()
{
    return tabDonnees;
}


int DOMParser::getHauteur()
{
    return hauteur;
}

int DOMParser::getLongeur()
{
    return longueur;
}


