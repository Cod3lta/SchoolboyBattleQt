#include "startmenu.h"

#include <QApplication>
#include <QBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QValidator>
#include <QLabel>
#include <QColorDialog>

StartMenu::StartMenu(QWidget *parent) : QWidget(parent)
{
    //setFixedSize(700,500);

    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::lightGray);
    setAutoFillBackground(true);
    setPalette(pal);

    // Création du layout
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    QHBoxLayout *connectToServerLayout = new QHBoxLayout(this);
    QHBoxLayout *copyrightLayout = new QHBoxLayout(this);
    QHBoxLayout *startLayout = new QHBoxLayout(this);
    QHBoxLayout *quitLayout = new QHBoxLayout(this);
    QHBoxLayout *imgLogoJeuLayout = new QHBoxLayout(this);
    QHBoxLayout *imgPersonnageLayout = new QHBoxLayout(this);

    // Création des widgets

    QPixmap pixmapPersonnage(":/Resources/brand/Personnage.png");
    QLabel *imgPersonnage=new QLabel;
    imgPersonnage->setPixmap(pixmapPersonnage);
    QPixmap pixmapLogoJeu(":/Resources/brand/logoJeu.PNG");
    QLabel *imgLogoJeu=new QLabel;
    imgLogoJeu->setPixmap(pixmapLogoJeu);
    QPushButton *startGame = new QPushButton("Démarrer le jeu en local à 2 joueurs");
    startGame->setStyleSheet("QPushButton {background-color: black; color: white;font-family:Algerian; font-size:12pt;}");
    QPushButton *connectToServer = new QPushButton("Se connecter à un serveur");
    connectToServer->setStyleSheet("QPushButton {background-color: black; color: white;font-family:Algerian; font-size:12pt;}");
    serverAddress = new QLineEdit("127.0.0.1", this);
    serverPort = new QLineEdit("1962", this);
    serverAddress->setPlaceholderText("Adresse du serveur");
    serverPort->setPlaceholderText("Port");
    QPushButton *btnQuit = new QPushButton("Quitter le jeu");
    btnQuit->setStyleSheet("QPushButton {background-color: black; color: white;font-family:Algerian; font-size:12pt;}");
    QLabel *lblCopyright = new QLabel("HE-ARC Copyright © 2021\nPrétat Valentin, Margueron Yasmine et Badel Kevin", this);
    lblCopyright->setAlignment(Qt::AlignHCenter);


    copyrightLayout->addStretch(1);
    copyrightLayout->addWidget(lblCopyright);
    imgLogoJeuLayout->addStretch(1);
    imgLogoJeuLayout->addWidget(imgLogoJeu);
    imgLogoJeuLayout->addStretch(1);
    imgPersonnageLayout->addStretch(1);
    imgPersonnageLayout->addWidget(imgPersonnage);
    imgPersonnageLayout->addStretch(1);
    startLayout->addWidget(startGame);
    quitLayout->addWidget(btnQuit);

    setFieldsValidator();

    vLayout->addStretch(1);
    vLayout->addLayout(imgLogoJeuLayout);
    vLayout->addLayout(imgPersonnageLayout);
    vLayout->addLayout(startLayout);
    connectToServerLayout->addWidget(serverAddress);
    connectToServerLayout->addWidget(serverPort);
    connectToServerLayout->addWidget(connectToServer);
    vLayout->addLayout(connectToServerLayout);
    vLayout->addLayout(quitLayout);
    vLayout->addStretch(1);
    vLayout->addLayout(copyrightLayout);

    setLayout(vLayout);

    // Connexions
    connect(startGame, &QPushButton::clicked, this, [=] () {
        emit setVisibleWidget(0);
        emit startLocalGame(2);
    });
    connect(connectToServer, &QPushButton::clicked, this, [=] () {
        if(serverAddress->text() == "" || serverPort->text() == "")
            return;
        emit setVisibleWidget(2);
        emit startClient(QHostAddress(serverAddress->text()), serverPort->text().toInt());
    });
    connect(btnQuit, &QPushButton::clicked, QApplication::instance(), &QApplication::quit);

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
