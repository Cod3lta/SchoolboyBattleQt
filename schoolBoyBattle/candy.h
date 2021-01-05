#ifndef CANDY_H
#define CANDY_H
#include <QGraphicsItem>
#include <QPixmap>

class Candy : public QGraphicsItem
{
public:
    Candy(int type, QGraphicsItem *parent = nullptr);

    enum Type :int {bleu = 0, rouge = 1, vert = 2, jaune = 3, orange = 4, rose = 5};
    typedef struct CandyDatas_s {
        QTimer *timer;
        QPixmap* image;
        int nbFrames;
        int frameIndex;
        int nbPoints;
    } CandyDatasStruct;
    static QHash<Type, CandyDatasStruct*> candiesDatas;
    static QHash<Type, CandyDatasStruct*> loadCandyDatas();
    static Candy::CandyDatasStruct *setupCandyData(int framerate, int nbFrames, int nbPoints, QString filename);

private:
    Type type;
    int points;             // Nombre de points que vaut ce bonbon

};

#endif // CANDY_H
