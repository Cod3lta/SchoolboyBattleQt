#ifndef KEYINPUTS_H
#define KEYINPUTS_H

#include <QMainWindow>
#include <QGraphicsItem>

class KeyInputs : public QGraphicsObject
{
    Q_OBJECT
public:
    KeyInputs(QGraphicsObject *parent = nullptr);
    ~KeyInputs();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void keyPress(QKeyEvent *event);
    void keyRelease(QKeyEvent *event);

signals:
    void playerKeyToggle(int playerId, int direction, bool value);

private:
    QHash<int, QList<int>> playersKeys;
    // La même chose se trouve dans player.h
    enum PlayerMovesEnum : int {up = 0, right = 1, down = 2, left = 3};
    void setPlayerKeys();
};

#endif // KEYINPUTS_H
