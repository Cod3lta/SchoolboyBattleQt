#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "gamewidget.h"
#include "tcpclient.h"
#include "waitingroom.h"

#include <QStackedWidget>

class MainWidget : public QStackedWidget
{
    Q_OBJECT
public:
    MainWidget();

private:
    GameWidget *gameWidget;
    WaitingRoom *waitingRoom;
    TcpClient *tcpClient;

private slots:

public slots:
//    void startLocal(int nbPlayers);
//    void startServer();
//    void startClient();

};

#endif // MAINWIDGET_H
