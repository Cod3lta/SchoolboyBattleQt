#include "viewscontainer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    int nbPlayers = 1;
    QApplication a(argc, argv);
    ViewsContainer v(nbPlayers);
    //v.showFullScreen();
    v.show();
    return a.exec();
}
