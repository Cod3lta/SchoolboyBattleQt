/*
 * Description : Cette classe s’occupe de de gérer les bonbons sur la partie.
 *               Chaque bonbon est soit présent sur le jeu n’appartenant
 *               à personne, soit dans la file derrière un joueur.
 *               Dès qu’un bonbon atteint la base d’une des deux équipes,
 *               il se fait valider puis supprimer de la scène.
 * Version     : 1.0.0
 * Date        : 25.01.2021
 * Auteurs     : Prétat Valentin, Badel Kevin et Margueron Yasmine
*/

#include "candy.h"
#include <QBrush>
#include <QPen>
#include <QDebug>
#include "game.h"

#define CANDY_WIDTH 130
#define CANDY_HEIGHT 130
#define HITBOX_DEBUG false
#define LERP_AMOUNT 1e7
#define LERP_ACCELERATION 20

/**
 * Constructeur des Candy afin de les initialiser.
 */
Candy::Candy(
        int candyType,
        int candySize,
        int nbPoints,
        DataLoader *dataLoader,
        TileCandyPlacement *tilePlacement,
        int id,
        QGraphicsObject *parent)
    : QGraphicsObject(parent),
      id(id),
      nbPoints(nbPoints),
      idTeam(-1),
      candyType(static_cast<Type>(candyType)),
      candySize(static_cast<Size>(candySize)),
      dataLoader(dataLoader),
      tilePlacement(tilePlacement),
      currentPlayerId(-1),
      taken(false),
      valid(false)
{
    loadAnimations();
    setAnimation(idle);
    setPos(tilePlacement->pos());
    setZIndex();
    if(tilePlacement != nullptr) {
        connect(this, &Candy::pickedUp, tilePlacement, &TileCandyPlacement::candyPickedUp);
    }
}

// Setup des animations des candies ---------------------------------------------------------
/**
 * Chargement des animations
 */
void Candy::loadAnimations() {
    animationsLocal.insert(
                idle,
                setupCandyAnimationData(
                    dataLoader->candyAnimations.value(
                        dataLoader->getCandyAnimationId(candyType, candySize))));
}

/**
 * Chargement des animations avec DataLoader.
 */
Candy::AnimationsLocalStruct* Candy::setupCandyAnimationData(DataLoader::CandyAnimationsStruct* sharedDatas) {
    AnimationsLocalStruct *c = new AnimationsLocalStruct();
    c->frameIndex = 0;
    c->sharedDatas = sharedDatas;
    // Si on donne un -1 pour le framerate, il n'y a pas d'animation
    c->timer = new QTimer();
    if(sharedDatas->framerate >= 0) {
        c->timer->setInterval(sharedDatas->framerate);
        c->timer->stop();
    }
    connect(c->timer, &QTimer::timeout, this, &Candy::animationNextFrame);
    return c;
}

// Setup des placements des candies ---------------------------------------------------------


// autres trucs de la classe ----------------------------------------------------------------

void Candy::animationNextFrame() {
    AnimationsLocalStruct *a = animationsLocal.value(animation);
    a->frameIndex++;
    if(a->frameIndex >= a->sharedDatas->nbFrame) {
        a->frameIndex = 0;
    }
    update();
}

/**
 * Lancer l'animation en gérant le timer.
 */
void Candy::setAnimation(Animations a) {
    // Arrêter le timer de l'animation qui se termine
    if(animationsLocal.contains(animation)) {
        animationsLocal.value(animation)->timer->stop();
    }

    // Changer l'animation
    animation = a;

    // Démmarer le timer de la nouvelle animation
    animationsLocal.value(a)->timer->start();
}

/**
 * Définition du Z Index
 */
void Candy::setZIndex() {
    setZValue(y() + CANDY_HEIGHT * 0.8);
}

/**
 * Le candy a été ramassé.
 */
void Candy::pickUp(int playerId, int idTeam) {
    taken = true;
    // Envoyer un msg au slot du TileCandyPlacement qu'il
    // peut en placer un nouveau
    emit pickedUp();
    currentPlayerId = playerId;
    this->idTeam = idTeam;
}

/**
 * Retourne si le Candy est pris par un joueur ou non.
 */
bool Candy::isTaken() {
    return taken;
}

/**
 * Retourne l'identifiant du Candy.
 */
int Candy::getId() {
    return id;
}

/**
 * Retourne le nombre de point du bonbon.
 */
int Candy::getNbPoints(){
    return nbPoints;
}

/**
 * Définir à quel joueur appartient ce Candy.
 */
void Candy::setCurrentPlayerId(int playerId) {
    currentPlayerId = playerId;
}

/**
 * Définir l'id de l'équipe que a le bonbon.
 */
void Candy::setTeamId(int idTeam) {
    this->idTeam = idTeam;
}

/**
 * Candy validé dans une équipe.
 */
void Candy::validate() {
    valid = true;
}

/**
 * Retourne si le Candy est validé ou non.
 */
bool Candy::isValidated() {
    return valid;
}

/**
 * Retourn l'id du joueur qui est en possession du bonbon.
 */
int Candy::getCurrentPlayerId() {
    return currentPlayerId;
}

/**
 * Mise à jour des candy.
 */
void Candy::refresh(QPointF pos, int posInQueue, double delta) {
    if(!taken || valid) return;
    int yOffset = 0;
    int lerpFactor = (LERP_AMOUNT * LERP_ACCELERATION) / (LERP_AMOUNT + posInQueue);
    if(posInQueue == 0) yOffset = dataLoader->getPlayerSize().y() / 8;
    setX(this->x() + (pos.x() - this->x()          ) / lerpFactor * delta);
    setY(this->y() + (pos.y() - this->y() + yOffset) / lerpFactor * delta);

    setZIndex();
}

/**
 * Prise du bonbon selon delta en millisecondes.
 */
void Candy::capture(double deltaMs) {
    QPoint objective = dataLoader->getTeamSpawnpoint(idTeam);
    setX(x() + (objective.x() - x()) / 20 * deltaMs);
    setY(y() + (objective.y() - y()) / 20 * deltaMs);

    // Si le candy est suffisamment proche du point de spawn
    if(x() - 50 < objective.x() && x() + 50 > objective.x() &&
            y() - 50 < objective.y() && y() + 50 > objective.y()) {
        emit validated(id, currentPlayerId);
    }
}

// OVERRIDE REQUIRED ------------------------------------------------------------------------

/**
 * Dessine le contenu du candy en coordonnées locales.
 */
void Candy::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    if(HITBOX_DEBUG) {
        // Debug rect
        painter->setPen(QPen(Qt::yellow));
        painter->drawRect(boundingRect());
        painter->setPen(QPen(Qt::red));
        painter->drawPath(shape());
        painter->drawText(10, 10, "ID : " + QString::number(id));
        painter->drawText(10, 30, "Player : " + QString::number(currentPlayerId));
    }

    AnimationsLocalStruct *candyToDraw = animationsLocal.value(animation);
    QPixmap *imageToDraw = candyToDraw->sharedDatas->image;
    QPixmap *imageHover = candyToDraw->sharedDatas->image;
    if(idTeam == 0)
        imageHover = candyToDraw->sharedDatas->imageRed;
    else if(idTeam == 1)
        imageHover = candyToDraw->sharedDatas->imageBlack;

    QRectF sourceRect = QRectF(imageToDraw->width() / candyToDraw->sharedDatas->nbFrame * candyToDraw->frameIndex, 0,
                               imageToDraw->width() / candyToDraw->sharedDatas->nbFrame, imageToDraw->height());
    QRectF targetRect = boundingRect();

    painter->drawPixmap(targetRect, *imageToDraw, sourceRect);
    if(idTeam != -1) painter->drawPixmap(targetRect, *imageHover, sourceRect);

    // Lignes pour le compilateur
    Q_UNUSED(option)
    Q_UNUSED(widget)
}


/**
 * Retourne les limites extérieures du candy sous forme de rectangle.
 */
QRectF Candy::boundingRect() const {
    return QRectF(0, 0, CANDY_WIDTH, CANDY_HEIGHT);
}

/**
 * Détection de collisions.
 */
QPainterPath Candy::shape() const {
    QPainterPath path;
    path.addRect(QRectF(
                     boundingRect().x() + boundingRect().width() / 4,
                     boundingRect().y() + boundingRect().height() / 4,
                     boundingRect().width()/2,
                     boundingRect().height()/2));
    return path;
}
