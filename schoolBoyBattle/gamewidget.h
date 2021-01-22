#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "game.h"
#include "tcpclient.h"
#include "view.h"

#include <QProgressBar>
#include <QWidget>

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameWidget(TcpClient *tcpClient, QWidget *parent = nullptr);
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    TcpClient *tcpClient;
    QProgressBar *teamsPointsProgess;
    QList<QGraphicsView*> views;
    Game *game;

public slots:
    void restartGame(int nbPlayers, int nbViews = 0);

private slots:
    void refreshGuiScore(int nbPointsRed, int nbPointsBlack);

signals:

};

#endif // GAMEWIDGET_H
