#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tcpserver.h"

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_DISABLE_COPY(MainWindow)

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QPlainTextEdit *editText;
    QPushButton *btnToggleServer;
    TcpServer *server;

private slots:
    void logMessage(const QString &msg);
    void toggleServer();
};
#endif // MAINWINDOW_H
