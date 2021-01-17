#include "mainwindow.h"

#include <QBoxLayout>
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      server(new TcpServer(this))
{
    // Construction du widget
    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *vLayout = new QVBoxLayout(mainWidget);
    QHBoxLayout *btnLayout = new QHBoxLayout(mainWidget);

    editText = new QPlainTextEdit;
    editText->setFont(*(new QFont("Courier New", 10, QFont::Bold)));
    btnToggleServer = new QPushButton("Démarrer");

    btnLayout->addStretch(1);
    btnLayout->addWidget(btnToggleServer);
    vLayout->addWidget(editText);
    vLayout->addLayout(btnLayout);
    mainWidget->setLayout(vLayout);
    setCentralWidget(mainWidget);

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

