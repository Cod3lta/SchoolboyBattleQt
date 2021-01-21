#include "finishmenu.h"
#include <QBoxLayout>

FinishMenu::FinishMenu(QWidget *parent) : QWidget(parent)
{
    int victoire=1;


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

     QLabel *labelInfos = new QLabel("");
     QLabel *imgVictory=new QLabel;
     if(victoire)
     {
         btnReturnMenu->setStyleSheet("QPushButton {background-color: Black; color: white;font-family:Algerian; font-size:12pt;}");
         labelInfos->setText("The black winner");
         labelInfos->setStyleSheet("QLabel { color: black; font-size:36pt;  }");
         QPixmap pixmapVictory(":/Resources/brand/black.png");
         imgVictory->setPixmap(pixmapVictory);
     }
     else
     {
         btnReturnMenu->setStyleSheet("QPushButton {background-color: Red; color: white;font-family:Algerian; font-size:12pt;}");
         labelInfos->setText("The red winner");
         labelInfos->setStyleSheet("QLabel { color: red; font-size:36pt;  }");
         QPixmap pixmapVictory(":/Resources/brand/red.png");
         imgVictory->setPixmap(pixmapVictory);
     }

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
         emit setVisibleWidget(1);
     });

}
