#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "game.h"
#include "tcpclient.h"
#include "view.h"

#include <QWidget>

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameWidget(TcpClient *tcpClient, QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    TcpClient *tcpClient;
    QList<QGraphicsView*> views;
    Game *game;

public slots:
    void restartGame(int nbPlayers, int nbViews = 0);

signals:

};

#endif // GAMEWIDGET_H
