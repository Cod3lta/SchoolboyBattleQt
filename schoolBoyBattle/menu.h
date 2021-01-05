#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class Menu : public QWidget
{
    Q_OBJECT
public:
    explicit Menu(QWidget *parent = 0);
    void afficherMenuPrincipal();

public slots:
    void aide();

private:
    QPushButton *btnNouvellePartie;
    QPushButton *btnAide;
    QPushButton *btnQuitter;
    QLabel *lblText;
    QLabel *lblCopyright;
    void parameters();


};

#endif // MENU_H
