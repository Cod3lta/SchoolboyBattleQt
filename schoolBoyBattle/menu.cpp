#include "menu.h"
#include <QVBoxLayout>

Menu::Menu(QWidget *parent) : QWidget(parent)
{
    lblText = new QLabel("Menu Principal", this);
    lblText->setAlignment(Qt::AlignHCenter);
    btnNouvellePartie = new QPushButton("Nouvelle Partie", this);
    btnAide = new QPushButton("Aide",this);
    btnQuitter = new QPushButton("Quitter",this);
    lblCopyright = new QLabel("HE-ARC Copyright 2021\nKevin Badel, Valentin Prétat et Yasmine Margueron", this);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(lblText);
    layout->addWidget(btnNouvellePartie);
    layout->addWidget(btnAide);
    layout->addWidget(btnQuitter);
    layout->addWidget(lblCopyright);

    setLayout(layout);

    connect(btnNouvellePartie, SIGNAL(clicked()), parent, SLOT(nouvellePartie()));
    connect(btnAide, SIGNAL(clicked()), this, SLOT(aide()));
    connect(btnQuitter, SIGNAL(clicked()), parent, SLOT(quitterJeu()));

}

void Menu::afficherMenuPrincipal()
{
    lblText->setText("MENU PRINCIPAL");
    this->setWindowTitle("Menu Principal");
    this->show();
}

void Menu::aide()
{
    lblText->setText("AIDE\n voir fichier txt ou ecrire les commandes ici"); //TODO
}
