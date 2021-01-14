
#include "stackedwidget.h"

#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    srand(time(NULL));
    QApplication a(argc, argv);

    QMainWindow w;
    w.setWindowTitle("SchoolBoyBattle");
    w.setWindowIcon(QIcon(":/Resources/schoolboybattle-icon.ico"));
    w.setCentralWidget(new StackedWidget);

    //v.showFullScreen();
    w.show();
    return a.exec();
}
