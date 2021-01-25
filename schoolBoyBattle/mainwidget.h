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
    QMediaPlayer *menuMusicPlayer;

private slots:
    void stopMenuMusic();
    void startMenuMusic();

};

#endif // MAINWIDGET_H
