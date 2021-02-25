/*
 * Description : Ceci est le widget de fin de partie, affichant le gagnant de la partie.
 *               Un bouton est présent pour permettre aux joueurs de
 *               retourner au menu principal.
 * Version     : 1.0.0
 * Date        : 25.01.2021
 * Auteurs     : Prétat Valentin, Badel Kevin et Margueron Yasmine
*/

#include "finishmenu.h"
#include <QBoxLayout>
#include <QTimer>

/**
 * Constructeur du menu de fin.
 */
FinishMenu::FinishMenu(QWidget *parent) : QWidget(parent) {
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::lightGray);
    setAutoFillBackground(true);
    setPalette(pal);

    QVBoxLayout *vLayout = new QVBoxLayout();
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QHBoxLayout *labelLayout = new QHBoxLayout();
    QHBoxLayout *victoryLayout = new QHBoxLayout();
    QHBoxLayout *mainLayout = new QHBoxLayout();
    QPushButton *btnReturnMenu = new QPushButton("Retour menu");

    labelInfos = new QLabel();
    imgVictory = new QLabel;

    victoryLayout->addStretch(1);
    victoryLayout->addWidget(imgVictory);
    victoryLayout->addStretch(1);
    labelLayout->addStretch(1);
    labelLayout->addWidget(labelInfos);
    labelLayout->addStretch(1);
    btnLayout->addStretch(1);
    btnLayout->addWidget(btnReturnMenu);
    btnLayout->addStretch(1);
    vLayout->addStretch(1);
    vLayout->addLayout(victoryLayout);
    vLayout->addLayout(labelLayout);
    vLayout->addStretch(1);
    vLayout->addLayout(btnLayout);
    vLayout->addStretch(1);
    mainLayout->addStretch(1);
    mainLayout->addLayout(vLayout);
    mainLayout->addStretch(1);
    setLayout(mainLayout);

    connect(btnReturnMenu, &QPushButton::clicked, this, [=] () {
        emit resetGame();
        emit updateStartMenuButtons();
        emit setVisibleWidget(1);
    });
}

/**
 * Permet d'afficher le vainqueur.
 */
void FinishMenu::showWinner(int teamWinner) {
    QPixmap pixmapVictory;

    switch (teamWinner) {
    case 0:
        labelInfos->setText("L'équipe rouge a gagné !");
        labelInfos->setStyleSheet(""
                                  "QLabel { color: #AE3838;"
                                  "font-family: Helvetica;"
                                  "color: #26292d;"
                                  "font-size: 35pt;"
                                  "font-weight: bold;}");
        pixmapVictory.load(":/Resources/brand/red.png");
        imgVictory->setPixmap(pixmapVictory);
        break;

    case 1:
        labelInfos->setText("L'équipe noir a gagné !");
        labelInfos->setStyleSheet(""
                                  "font-family: Helvetica;"
                                  "color: #26292d;"
                                  "font-size: 35pt;"
                                  "font-weight: bold;}");
        pixmapVictory.load(":/Resources/brand/black.png");
        imgVictory->setPixmap(pixmapVictory);
        break;

    case -1:
        labelInfos->setText("Ex-aeco !");
        labelInfos->setStyleSheet(""
                                  "QLabel { color: #AE3838;"
                                  "font-family: Helvetica;"
                                  "color: #26292d;"
                                  "font-size: 35pt;"
                                  "font-weight: bold;}");
        pixmapVictory.load(":/Resources/brand/Personnage.png");
        imgVictory->setPixmap(pixmapVictory);
        break;
    }

    // Démarrer la musique du menu après 3 secondes
    QTimer timer(this);
    timer.singleShot(3000, this, &FinishMenu::startMenuMusic);
}
