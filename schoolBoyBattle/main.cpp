#include "viewscontainer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int nbPlayers = 2;
    QApplication a(argc, argv);

    ViewsContainer v(nbPlayers);
    //v.showFullScreen();
    v.show();
    return a.exec();
}
