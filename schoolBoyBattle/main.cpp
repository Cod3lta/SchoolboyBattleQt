#include "viewscontainer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    //QGuiApplication a(argc, argv);
    QApplication a(argc, argv);
    int nbPlayers = 2;

    ViewsContainer v(nbPlayers);
    v.show();

    //v.showFullScreen();


//    Niveau newNiveau("terrain1.tmx");
//    newNiveau.show();


    return a.exec();

}
