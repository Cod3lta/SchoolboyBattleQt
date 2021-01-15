#include "startmenu.h"

#include <QApplication>
#include <QBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QValidator>

StartMenu::StartMenu(QWidget *parent) : QWidget(parent)
{
    // Création du layout
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    QHBoxLayout *connectToServerLayout = new QHBoxLayout(this);

    QPushButton *startGame = new QPushButton("Démarrer le jeu en local à 2 joueurs");
    QPushButton *connectToServer = new QPushButton("Se connecter à un serveur");
    serverAddress = new QLineEdit(this);
    serverPort = new QLineEdit(this);
    serverAddress->setPlaceholderText("Adresse du serveur");
    serverPort->setPlaceholderText("Port");
    QPushButton *btnQuit = new QPushButton("Quitter le jeu");
    setFieldsValidator();

    vLayout->addStretch(1);
    vLayout->addWidget(startGame);
    connectToServerLayout->addWidget(serverAddress);
    connectToServerLayout->addWidget(serverPort);
    connectToServerLayout->addWidget(connectToServer);
    vLayout->addLayout(connectToServerLayout);
    vLayout->addWidget(btnQuit);
    vLayout->addStretch(1);

    setLayout(vLayout);

    // Connexions
    connect(startGame, &QPushButton::pressed, this, [=] () {
        emit setVisibleWidget(0);
        emit startLocalGame(2);
    });
    connect(connectToServer, &QPushButton::pressed, this, [=] () {
        if(serverAddress->text() == "" || serverPort->text() == "")
            return;
        emit setVisibleWidget(2);
        emit startClient(QHostAddress(serverAddress->text()), serverPort->text().toInt());
    });
    connect(btnQuit, &QPushButton::pressed, QApplication::instance(), &QApplication::quit);
}

void StartMenu::setFieldsValidator() {
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    serverAddress->setValidator(ipValidator);
    serverPort->setValidator( new QIntValidator(0, 9999, this) );
}
