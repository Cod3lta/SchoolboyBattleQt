#include "game.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Game g;
    g.showFullScreen();
    g.show();
    return a.exec();
}
