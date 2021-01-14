#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "game.h"
#include "view.h"

#include <QWidget>

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    QList<QGraphicsView*> views;
    Game *game;

public slots:
    void createLocalGame();

signals:

};

#endif // GAMEWIDGET_H
