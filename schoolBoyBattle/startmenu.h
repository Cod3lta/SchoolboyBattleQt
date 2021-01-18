#ifndef STARTMENU_H
#define STARTMENU_H

#include <QDialog>
#include <QHostAddress>
#include <QLineEdit>
#include <QSignalMapper>


class StartMenu : public QWidget
{
    Q_OBJECT
public:
    explicit StartMenu(QWidget *parent = nullptr);

signals:
    void startLocalGame(int nbPlayers, int nbViews);
    void startServer();
    void startClient(QHostAddress address, int port);
    void setVisibleWidget(int i);

private:
    QSignalMapper *signalMapper;
    void setFieldsValidator();
    QLineEdit *serverAddress;
    QLineEdit *serverPort;

};

#endif // STARTMENU_H
