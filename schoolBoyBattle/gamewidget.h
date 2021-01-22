#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "game.h"
#include "tcpclient.h"
#include "view.h"

#include <QBoxLayout>
#include <QLabel>
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
    QList<QGraphicsView*> views;
    Game *game;
    QBoxLayout *hlayout;
    // GUI
    QProgressBar *teamsPointsProgess;
    QLabel *pointsRed;
    QLabel *pointsBlack;

public slots:
    void restartGame(int nbPlayers, int nbViews = 0);

private slots:
    void refreshGuiScore(int nbPointsRed, int nbPointsBlack);

signals:
    void showWinner(int teamWinner);
    void setVisibleWidget(int index);

};

#endif // GAMEWIDGET_H
