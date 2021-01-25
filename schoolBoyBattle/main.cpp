/*
 * Description : Cette classe permet de lancer notre application.
 * Version     : 1.0.0
 * Date        : 25.01.2021
 * Auteurs     : Prétat Valentin, Badel Kevin et Margueron Yasmine
*/

#include "mainwidget.h"
#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    srand(time(NULL));
    QApplication a(argc, argv);

    QMainWindow w;
    w.setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    w.setWindowTitle("SchoolBoyBattle");
    w.setWindowIcon(QIcon(":/Resources/brand/schoolboybattle-icon.ico"));
    w.setCentralWidget(new MainWidget);

    w.show();
    return a.exec();
}
