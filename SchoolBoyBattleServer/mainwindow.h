/*
 * Description : Cette classe s'occupe de créer l’interface du serveur.
 *               Elle possède un objet QPlainTextEdit qui montre les arrivées
 *               et envois de tous les messages que le serveur traite.
 * Version     : 1.0.0
 * Date        : 25.01.2021
 * Auteurs     : Prétat Valentin, Badel Kevin et Margueron Yasmine
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tcpserver.h"

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_DISABLE_COPY(MainWindow)

public:
    MainWindow(QWidget *parent = nullptr);

private:
    QPlainTextEdit *editText;
    QPushButton *btnToggleServer;
    TcpServer *server;

private slots:
    void logMessage(const QString &msg);
    void toggleServer();
};
#endif // MAINWINDOW_H
