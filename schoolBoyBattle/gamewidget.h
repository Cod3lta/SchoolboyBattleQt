#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "game.h"
#include "tcpclient.h"
#include "view.h"

#include <QBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QWidget>
#include <QTimer>
#include <QMediaPlayer>

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    GameWidget(TcpClient *tcpClient, QWidget *parent = nullptr);
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    TcpClient *tcpClient;
    QList<View*> views;
    Game *game;
    QBoxLayout *viewsLayout;
    // GUI
    QProgressBar *teamsPointsProgess;
    QLabel *pointsRed;
    QLabel *pointsBlack;
    QMediaPlayer *ambientMusicPlayer;
    bool gameRunning;

public slots:
    void startGame(int nbPlayers, int nbViews = 0);
    void resetGame();

private slots:
    void updateTeamsPoints(int nbPointsRed, int nbPointsBlack);

signals:
    void setFinishMenuWinner(int teamWinner);
    void setVisibleWidget(int index);
    void stopMenuMusic();

};

#endif // GAMEWIDGET_H
