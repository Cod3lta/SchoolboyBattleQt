/*
 * Description : Ceci est le widget de fin de partie, affichant le gagnant de la partie.
 *               Un bouton est présent pour permettre aux joueurs de
 *               retourner au menu principal.
 * Version     : 1.0.0
 * Date        : 25.01.2021
 * Auteurs     : Prétat Valentin, Badel Kevin et Margueron Yasmine
*/

#include <QPushButton>
#include <QLabel>

#ifndef FINISHMENU_H
#define FINISHMENU_H

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
