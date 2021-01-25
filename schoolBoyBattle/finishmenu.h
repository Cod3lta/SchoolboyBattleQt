#ifndef FINISHMENU_H
#define FINISHMENU_H

#include <QPushButton>
#include <QLabel>



class FinishMenu : public QWidget
{
     Q_OBJECT
public:
   FinishMenu(QWidget *parent = nullptr);

private:
    QLabel *labelInfos;
    QLabel *imgVictory;

public slots:
    void showWinner(int teamWinner);

signals:
    void setVisibleWidget(int i);
    void resetGame();
    void startMenuMusic();
    void updateStartMenuButtons();
};

#endif // FINISHMENU_H
