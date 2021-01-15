#ifndef STARTMENU_H
#define STARTMENU_H

#include <QDialog>
#include <QSignalMapper>

class StartMenu : public QWidget
{
    Q_OBJECT
public:
    explicit StartMenu(QWidget *parent = nullptr);

signals:
    void startLocalGame(int i);
    void startServer();
    void startClient();
    void setVisibleWidget(int i);

private:
    QSignalMapper *signalMapper;

};

#endif // STARTMENU_H
