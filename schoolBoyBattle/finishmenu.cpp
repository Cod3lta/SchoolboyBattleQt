#include "finishmenu.h"
#include <QBoxLayout>

FinishMenu::FinishMenu(QWidget *parent) : QWidget(parent)
{
    startMenu = new StartMenu(this);


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
     btnReturnMenu->setStyleSheet("QPushButton {background-color: black; color: white;font-family:Algerian; font-size:12pt;}");
     QLabel *labelInfos = new QLabel("The red winners");
     labelInfos->setStyleSheet("QLabel { color: red; font-size:36pt;font-family:Algerian;  }");

     QPixmap pixmapVictory(":/Resources/brand/red.png");
     QLabel *imgVictory=new QLabel;
     imgVictory->setPixmap(pixmapVictory);
     victoryLayout->addStretch(1);
     victoryLayout->addWidget(imgVictory);
     victoryLayout->addStretch(1);
     labelLayout->addStretch(1);
     labelLayout->addWidget(labelInfos);
     labelLayout->addStretch(1);


     QPixmap pixmapFeu(":/Resources/brand/T2.png");
     QLabel *imgFeu=new QLabel;
     imgFeu->setPixmap(pixmapFeu);


     btnLayout->addStretch(1);
     btnLayout->addWidget(btnReturnMenu);
     btnLayout->addStretch(1);


     vLayout->addStretch(1);
     vLayout->addLayout(victoryLayout);
     vLayout->addLayout(labelLayout);
     vLayout->addLayout(btnLayout);
     vLayout->addStretch(1);
     mainLayout->addStretch(1);
     mainLayout->addLayout(vLayout);
     mainLayout->addStretch(1);
     setLayout(mainLayout);

     connect(btnReturnMenu, &QPushButton::clicked, this, [=] () {
         emit setVisibleWidget(1);
     });

}
