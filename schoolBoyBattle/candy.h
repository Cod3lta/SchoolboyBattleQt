#ifndef CANDY_H
#define CANDY_H
#include <QGraphicsObject>
#include <QPixmap>
#include "dataloader.h"
#include "tilecandyplacement.h"

class Candy : public QGraphicsObject
{
    Q_OBJECT
public:
    Candy(
            int candyType,
            int candySize,
            int nbPoints,
            DataLoader *dataLoader,
            TileCandyPlacement *tilePlacement,
            int id,
            QGraphicsObject *parent = nullptr);
    ~Candy();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void pickUp(int playerId, int idTeam);
    void refresh(QPointF pos, int posInQueue, double delta);
    void capture(double deltaMs);
    bool isTaken();
    int getId();
    int getNbPoints();
    int getCurrentPlayerId();
    void setCurrentPlayerId(int playerId);
    void setTeamId(int idTeam);
    void validate();
    bool isValidated();


    enum Type : int {peanut = 0, mandarin = 1};
    enum Size : int {small = 0, big = 1};
    enum Animations {idle = 0};

private:
    typedef struct Animations_s {
        QTimer *timer;
        int frameIndex;
        DataLoader::CandyAnimationsStruct *sharedDatas;
    } AnimationsLocalStruct;


    QHash<Animations, AnimationsLocalStruct*> animationsLocal;
    int id;
    int nbPoints;
    int idTeam;
    Type candyType;
    Size candySize;
    Animations animation;
    DataLoader *dataLoader;
    TileCandyPlacement* tilePlacement;
    int currentPlayerId;
    bool taken;
    bool valid;

    void loadAnimations();
    Candy::AnimationsLocalStruct *setupCandyAnimationData(DataLoader::CandyAnimationsStruct *sharedDatas);
    void setType(Type t);
    void animationNextFrame();
    void setAnimation(Animations a);
    void setZIndex();

signals:
    void pickedUp();
    void validated(int id, int playerId);
};


#endif // CANDY_H
