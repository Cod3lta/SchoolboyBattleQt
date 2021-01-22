#include "gamewidget.h"
#include "startmenu.h"

#include <QLabel>
#include <QKeyEvent>
#include <QDebug>
#include <QtMultimedia/QMediaPlayer>
#include <QSound>
#include <QAudioOutput>

GameWidget::GameWidget(TcpClient *tcpClient, QWidget *parent) :
    QWidget(parent),
    tcpClient(tcpClient)
{

    teamsPointsProgess = new QProgressBar(this);
    teamsPointsProgess->setMinimum(0);
    teamsPointsProgess->setMaximum(100);
    teamsPointsProgess->setValue(50);
    teamsPointsProgess->setTextVisible(false);
    pointsRed = new QLabel("points rouges", this);
    pointsBlack = new QLabel("points noirs", this);
    teamsPointsProgess->resize(size().width() * 0.5, 15);
    teamsPointsProgess->move((width() - teamsPointsProgess->width())/2, 50);
    pointsRed->setAlignment(Qt::AlignmentFlag::AlignRight);

}

void GameWidget::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event)
    teamsPointsProgess->resize(size().width() * 0.5, 15);
    teamsPointsProgess->move((width() - teamsPointsProgess->width())/2, 50);
    pointsRed->move(teamsPointsProgess->pos().x() - 10, 50);
    pointsBlack->move(teamsPointsProgess->pos().x() + teamsPointsProgess->width() + 10, 50);
}

void GameWidget::keyPressEvent(QKeyEvent *event) {
    if(event->isAutoRepeat()) {
        event->ignore();
        return;
    }
    game->keyPress(event);
}

void GameWidget::keyReleaseEvent(QKeyEvent *event) {
    if(event->isAutoRepeat()) {
        event->ignore();
        return;
    }
    game->keyRelease(event);
}

void GameWidget::restartGame(int nbPlayers, int nbViews) {
    game = new Game();
    connect(game, &Game::teamsPointsChanged, this, &GameWidget::refreshGuiScore);
    connect(game, &Game::showEndScreen, this, [=] (int teamWinner) {
        delete game;
        for(int i = 0; i < views.length(); i++)
            delete views.at(i);
        views.clear();
        delete hlayout;
        emit showWinner(teamWinner);
        emit setVisibleWidget(3);
    });

    if(nbViews == 0) nbViews = nbPlayers;
    // S'il y a autant de QGraphicsView que de joueurs -> splitscreen
    bool isMultiplayer = nbPlayers == nbViews ? false : true;
    QString terrainFileName = ":/Resources/debugTerrain.tmx";
    game->startGame(terrainFileName, nbPlayers, isMultiplayer, tcpClient);
    hlayout = new QHBoxLayout(this);

    for(int i = 0; i < nbViews; i++) {
        View *v = new View(i);
        v->setScene(game);
        hlayout->addWidget(v);
        views.append(v);
    }

    setLayout(hlayout);
    teamsPointsProgess->raise();
    pointsRed->raise();
    pointsBlack->raise();
    setFocusPolicy(Qt::StrongFocus);
    setFocus();


    QMediaPlayer *ambientMusicPlayer = new QMediaPlayer(this);
    ambientMusicPlayer->setMedia(QUrl("qrc:/Resources/sounds/mainTitle.wav"));
    ambientMusicPlayer->play();
    //QSound music("qrc:/Resources/music.wav");
    //music.play();
//    QFile musicSourceFile;
//    QAudioOutput* audio;


}

void GameWidget::refreshGuiScore(int nbPointsRed, int nbPointsBlack) {
    teamsPointsProgess->setMaximum(nbPointsRed + nbPointsBlack);
    teamsPointsProgess->setValue(nbPointsRed);
    pointsRed->setText(QString::number(nbPointsRed));
    pointsBlack->setText(QString::number(nbPointsBlack));
}

