/*
 * Description : Cette classe s'occupe de créer le widget affichant l’écran
 *               de préparation au mode de jeu multijoueur.
 *               Ce widget sert à accueillir tous les joueurs souhaitant
 *               jouer ensemble avant de lancer la partie.
 * Version     : 1.0.0
 * Date        : 25.01.2021
 * Auteurs     : Prétat Valentin, Badel Kevin et Margueron Yasmine
*/

#include "tcpclient.h"
#include <QBoxLayout>
#include <QHostAddress>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

#ifndef WAITINGROOM_H
#define WAITINGROOM_H

class WaitingRoom : public QWidget
{
    Q_OBJECT

public:
    WaitingRoom(TcpClient *tcpClient, QWidget *parent = nullptr);

private:
    TcpClient* tcpClient;
    QLabel *mainLabel;
    QList<QHBoxLayout *> usersLayout;
    QList<QLabel *> usersName;
    QList<QLabel *> usersReady;
    QPushButton *btnReady;
    QPushButton *btnLeave;

private slots:
    void userListRefresh(QHash<int, QHash<QString, QString>>);
    void connected();

public slots:
    void startWaitingRoom(QHostAddress address, qint16 port);

signals:
    void setVisibleWidget(int i);
};

#endif // WAITINGROOM_H
