/*
 * Description : Cette  classe ne crée qu’un seul objet.
 *               Son utilité est de pouvoir capturer les événements du clavier
 *               au même endroit avant de les envoyer aux objets Player correspondants.
 * Version     : 1.0.0
 * Date        : 25.01.2021
 * Auteurs     : Prétat Valentin, Badel Kevin et Margueron Yasmine
*/

#ifndef KEYINPUTS_H
#define KEYINPUTS_H

#include <QMainWindow>
#include <QGraphicsItem>

class KeyInputs : public QGraphicsObject
{
    Q_OBJECT
public:
    KeyInputs(int focusedPlayerId = 0, QGraphicsObject *parent = nullptr);
    ~KeyInputs();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void keyPress(QKeyEvent *event);
    void keyRelease(QKeyEvent *event);

private:
    QHash<int, QList<int>> playersKeys;
    // La même chose se trouve dans player.h
    void setPlayerKeys(int focusedPlayerId);
    enum PlayerMovesEnum : int {up = 0, right = 1, down = 2, left = 3};

signals:
    void playerKeyToggle(int playerId, int direction, bool value);
};

#endif // KEYINPUTS_H
