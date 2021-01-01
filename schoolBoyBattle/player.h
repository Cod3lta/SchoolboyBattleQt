#ifndef PLAYER_H
#define PLAYER_H

#include <QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>



class Player : public QGraphicsObject
{
public:
    Player(int id, int team, QGraphicsObject *parent = nullptr);
    ~Player();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void move();

private:
    enum Team : int {red = 0, black = 1};
    enum Gender : int {girl = 0, boy = 1};
    enum Animation : int {idle = 0, run = 1};
    enum PlayerMovesEnum : int {up = 0, right = 1, down = 2, left = 3};

    Team team;
    Gender gender;
    int id;
    bool moves[4] = {false, false, false, false};
    Animation currentAnimation;
    QRectF hitbox;

    QHash<Team, QList<int>> teamsSpawnpoint;
    QGraphicsRectItem *debugRect;
    typedef struct Animations_s {
        QPixmap *image;
        QTimer *timer;
        int framerate;
        int nbFrame;
        // Pour avoir la taille d'une frame
        // -> image.width() / nbFrame;
    } AnimationsStruct;
    QHash<Animation, AnimationsStruct*> animations;

    void validate_candies();
    void takeCandy();
    void loadAnimations();
    void animationNextFrame();
    void setAnimation(Animation a);
    QPixmap *getAnimationByTeamAndGender(QString name);
    AnimationsStruct *setupAnimation(int framerate, int nbFrame, QString filename);
public slots:
    void keyMove(int playerId, int direction, bool value);

};

#endif // PLAYER_H
