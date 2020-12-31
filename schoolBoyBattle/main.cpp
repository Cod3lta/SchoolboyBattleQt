#include "viewscontainer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ViewsContainer v;
    //v.showFullScreen();
    v.show();
    return a.exec();
}
