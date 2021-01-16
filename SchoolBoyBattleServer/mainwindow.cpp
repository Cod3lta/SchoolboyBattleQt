#include "mainwindow.h"

#include <QVBoxLayout>
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      server(new TcpServer(this))
{
    //QVBoxLayout *vLayout = new QVBoxLayout(this);

    editText = new QPlainTextEdit(this);
    editText->resize(600, 300);
    editText->setFont(*(new QFont("Courier New", 10, QFont::Bold)));
    btnToggleServer = new QPushButton("Démarrer", this);
    btnToggleServer->move(0, 350);

    connect(btnToggleServer, &QPushButton::clicked, this, &MainWindow::toggleServer);
    connect(server, &TcpServer::logMessage, this, &MainWindow::logMessage);

    logMessage(QString("  _____________________________  _________\n") +
               " /   _____/\\______   \\______   \\/   _____/\n" +
               " \\_____  \\  |    |  _/|    |  _/\\_____  \\ \n" +
               " /        \\ |    |   \\|    |   \\/        \\\n" +
               "/_______  / |______  /|______  /_______  /\n" +
               "        \\/         \\/        \\/        \\/ \n");
    logMessage("---------------------\nSchoolBoyBattleServer\n---------------------");
}

MainWindow::~MainWindow()
{
}

void MainWindow::logMessage(const QString &msg)
{
    editText->appendPlainText(msg);
}

void MainWindow::toggleServer()
{
    if(server->isListening()) {
        server->stopServer();
        btnToggleServer->setText("Démarrer");
        logMessage("Server stoppé\n---------------------");
    }else {
        if(!server->listen(QHostAddress::Any, 1962)) {
            QMessageBox::critical(this, "Erreur", "Impossible de démarrer le serveur");
            return;
        }
        logMessage("\nServer démarré");
        logMessage("Adresse du serveur : " + server->serverAddress().toString());
        logMessage("Port : " + QString::number(server->serverPort()));
        btnToggleServer->setText("Arrêter");
    }
}

