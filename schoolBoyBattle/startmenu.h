#ifndef STARTMENU_H
#define STARTMENU_H

#include <QDialog>

class StartMenu : public QWidget
{
    Q_OBJECT
public:
    explicit StartMenu(QWidget *parent = nullptr);

private slots:
    void startLocalGame();

signals:
    void changeWidget(int i);

};

#endif // STARTMENU_H
