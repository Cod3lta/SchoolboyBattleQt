/*
 * Description : Cette  classe est le widget principal qui se trouve
 *               dans l’objet QMainWindow du programme.
 *               Cette classe hérite de QStackedWidget, ce qui lui permet
 *               d’afficher un objet QWidget à la fois, donnant une façon simple
 *               d’implémenter des menus d’interface où l’on passe d’une page à l’autre.
 *               Au lancement du programme, MainWidget met comme widget visible
 *               celui provenant de la classe StartMenu.
 * Version     : 1.0.0
 * Date        : 25.01.2021
 * Auteurs     : Prétat Valentin, Badel Kevin et Margueron Yasmine
*/

#include "gamewidget.h"
#include "tcpclient.h"
#include "waitingroom.h"
#include <QStackedWidget>

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

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
