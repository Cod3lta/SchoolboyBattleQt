#ifndef FINISHMENU_H
#define FINISHMENU_H

#include <QPushButton>
#include <QLabel>
#include "startmenu.h"


class FinishMenu : public QWidget
{
     Q_OBJECT
public:
   explicit FinishMenu(QWidget *parent = nullptr);
    StartMenu *startMenu;
signals:
    void setVisibleWidget(int i);
};

#endif // FINISHMENU_H
