#ifndef CANDY_H
#define CANDY_H
#include <QGraphicsItem>
#include <QPixmap>

class Candy : public QGraphicsItem
{
public:
    Candy(QGraphicsItem *parent = nullptr);

private:
    const static int nbCandies = 100;
    enum Type :int {bleu = 0, rouge = 1, vert = 2, jaune = 3, orange = 4, rose = 5};
    /*typedef struct CandyDatas_s {
        QPixmap* image;
    } CandyDatasStruct;*/
    static QHash<Type, QPixmap*> CandiesDatas;

    QTimer *newCandy;

};

#endif // CANDY_H
