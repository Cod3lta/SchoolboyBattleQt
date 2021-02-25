/*
 * Description : Ceci est le widget contenant les objets de la classe QGraphicsView
 *               et de la classe QGraphicsScene. Ce widget est visible à l’écran
 *               lorsque le jeu est lancé.
 * Version     : 1.0.0
 * Date        : 25.01.2021
 * Auteurs     : Prétat Valentin, Badel Kevin et Margueron Yasmine
*/

#include "game.h"
#include "tcpclient.h"
#include "view.h"
#include <QBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QWidget>
#include <QTimer>
#include <QMediaPlayer>

#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

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
    QLabel *timeLeft;
    int min, sec;
    QMediaPlayer *ambientMusicPlayer;
    QTimer *gameTimer;
    bool gameRunning;

public slots:
    void startGame(int nbPlayers, int nbViews = 0);
    void resetGame();

private slots:
    void updateTeamsPoints(int nbPointsRed, int nbPointsBlack);
    void timerDecreases();

signals:
    void setFinishMenuWinner(int teamWinner);
    void setVisibleWidget(int index);
    void stopMenuMusic();
};

#endif // GAMEWIDGET_H
