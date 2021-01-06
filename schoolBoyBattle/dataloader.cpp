#include "dataloader.h"
#include <QDebug>

DataLoader::DataLoader()
{
    loadPlayerAnimations();
}

// PLAYER -----------------------------------------------------------------------------------

void DataLoader::loadPlayerAnimations() {
    playerAnimations.insert(0, setupPlayerAnimation(6, ":/Resources/player/idle/boy-black-idle.png"));
    playerAnimations.insert(1, setupPlayerAnimation(6, ":/Resources/player/idle/girl-black-idle.png"));
    playerAnimations.insert(2, setupPlayerAnimation(6, ":/Resources/player/idle/boy-red-idle.png"));
    playerAnimations.insert(3, setupPlayerAnimation(6, ":/Resources/player/idle/girl-red-idle.png"));
    playerAnimations.insert(4, setupPlayerAnimation(10, ":/Resources/player/run/boy-black-run.png"));
    playerAnimations.insert(5, setupPlayerAnimation(10, ":/Resources/player/run/girl-black-run.png"));
    playerAnimations.insert(6, setupPlayerAnimation(10, ":/Resources/player/run/boy-red-run.png"));
    playerAnimations.insert(7, setupPlayerAnimation(10, ":/Resources/player/run/girl-red-run.png"));
}

DataLoader::PlayerAnimationsStruct* DataLoader::setupPlayerAnimation(int nbFrame, QString fileName) {
    PlayerAnimationsStruct* aStruct = new PlayerAnimationsStruct;
    aStruct->nbFrame = nbFrame;
    aStruct->image = new QPixmap(fileName);
    return aStruct;
}

int DataLoader::getPlayerAnimationId(int gender, int team, int animation) {
    // TODO : Améliorer ce code un peu moche
    if(gender == 0 && team == 0 && animation == 0) return 3;
    if(gender == 1 && team == 0 && animation == 0) return 2;
    if(gender == 0 && team == 1 && animation == 0) return 1;
    if(gender == 1 && team == 1 && animation == 0) return 0;
    if(gender == 0 && team == 0 && animation == 1) return 7;
    if(gender == 1 && team == 0 && animation == 1) return 6;
    if(gender == 0 && team == 1 && animation == 1) return 5;
    if(gender == 1 && team == 1 && animation == 1) return 4;
    return -1;
}

// CANDY ------------------------------------------------------------------------------------

void DataLoader::loadCandyAnimations() {
    candiesAnimations.insert(0, setupCandyAnimations(1, 1, ":/Resources/candy/peanut.png"));
    candiesAnimations.insert(1, setupCandyAnimations(1, 5, ":/Resources/candy/mandarin.png"));
}

DataLoader::CandyAnimationsStruct * DataLoader::setupCandyAnimations(int nbFrame, int nbPoints, QString filename) {
    CandyAnimationsStruct *c = new CandyAnimationsStruct();
    c->nbPoints = nbPoints;
    c->image = new QPixmap(filename);
    c->nbFrame = nbFrame;
    return c;

}

int DataLoader::getCandyAnimationId(int type) {
    // TODO : Améliorer ce code un peu moche
    if(type == 0) return 0;
    if(type == 1) return 1;
    return -1;
}
