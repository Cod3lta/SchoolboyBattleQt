#include "finishmenu.h"
#include <QBoxLayout>

FinishMenu::FinishMenu(QWidget *parent) : QWidget(parent) {
    int victoire = 1;

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

     QLabel *labelInfos = new QLabel();
     QLabel *imgVictory=new QLabel;

     if(victoire == 1) {
         labelInfos->setText("L'équipe noir a gagné !");
         labelInfos->setStyleSheet(""
            "font-family: Helvetica;"
            "color: #26292d;"
            "font-size: 35pt;"
            "font-weight: bold;}");
         QPixmap pixmapVictory(":/Resources/brand/black.png");
         imgVictory->setPixmap(pixmapVictory);
     }else if(victoire == 0) {
         labelInfos->setText("L'équipe rouge a gagné !");
         labelInfos->setStyleSheet(""
            "QLabel { color: #AE3838;"
            "font-family: Helvetica;"
            "color: #26292d;"
            "font-size: 35pt;"
            "font-weight: bold;}");
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
