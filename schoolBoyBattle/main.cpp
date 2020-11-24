#include "view.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    View v;
    v.showFullScreen();
    v.show();
    return a.exec();
}
