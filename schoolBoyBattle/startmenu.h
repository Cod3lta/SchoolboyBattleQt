/*
 * Description : Cette  classe crée le premier widget visible
 *               au lancement du programme, composé d’une interface simple
 *               pour choisir le mode de jeu.
 * Version     : 1.0.0
 * Date        : 25.01.2021
 * Auteurs     : Prétat Valentin, Badel Kevin et Margueron Yasmine
*/

#include <QDialog>
#include <QHostAddress>
#include <QLineEdit>

#ifndef STARTMENU_H
#define STARTMENU_H

class StartMenu : public QWidget
{
    Q_OBJECT

public:
    StartMenu(QWidget *parent = nullptr);

signals:
    void startLocalGame(int nbPlayers, int nbViews);
    void startServer();
    void startClient(QHostAddress address, int port);
    void setVisibleWidget(int i);

private:
    void setFieldsValidator();

    QLineEdit *serverAddress;
    QLineEdit *serverPort;
};

#endif // STARTMENU_H
