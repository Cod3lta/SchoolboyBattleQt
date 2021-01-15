#ifndef STACKEDWIDGET_H
#define STACKEDWIDGET_H

#include "gamewidget.h"

#include <QStackedWidget>

class StackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    StackedWidget();

private:
    GameWidget *gameWidget;

private slots:
//    void startLocal(int nbPlayers);
//    void startServer();
//    void startClient();

};

#endif // STACKEDWIDGET_H
