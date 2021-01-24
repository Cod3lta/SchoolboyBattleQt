#include "gamewidget.h"
#include "startmenu.h"

#include <QLabel>
#include <QKeyEvent>
#include <QDebug>
#include <QSound>
#include <QAudioOutput>

GameWidget::GameWidget(TcpClient *tcpClient, QWidget *parent) :
    QWidget(parent),
    tcpClient(tcpClient),
    gameRunning(false)
{
    ambientMusicPlayer = new QMediaPlayer(this);
    teamsPointsProgess = new QProgressBar(this);
    teamsPointsProgess->setMinimum(0);
    teamsPointsProgess->setMaximum(100);
    teamsPointsProgess->setValue(50);
    teamsPointsProgess->setTextVisible(false);
    teamsPointsProgess->setStyleSheet(""
    "QProgressBar {"
        "background-color: #1b1c1e;"
        "border: 1px solid black;"
        "text-align: top;"
        "padding: 1px;"
        "border-radius: 7px;"
        "width: 15px;"
    "}"
    "QProgressBar::chunk {"
        "background-color: #ae3838;"
        "border: 1px solid black;"
        "border-bottom-left-radius: 7px;"
        "border-top-left-radius: 7px;"
        "}");
    pointsRed = new QLabel("0", this);
    pointsBlack = new QLabel("0", this);
    teamsPointsProgess->resize(size().width() * 0.5, 15);
    teamsPointsProgess->move((width() - teamsPointsProgess->width())/2, 50);
    pointsRed->setAlignment(Qt::AlignmentFlag::AlignRight);

    setStyleSheet(""
        "QLabel {"
            "font-family: Helvetica;"
            "color: #d8d9e6;"
            "font-weight: bold;"
            "font-size: 35px;"
            "background-color: #1b1c1e;"
            "border: 3px solid black;"
            "border-radius: 7px;"
        "}");
}

void GameWidget::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event)
    teamsPointsProgess->resize(size().width() * 0.5, 15);
    teamsPointsProgess->move((width() - teamsPointsProgess->width())/2, 50);
    pointsRed->adjustSize();
    pointsRed->resize(100, pointsRed->size().height());
    pointsRed->setAlignment(Qt::AlignCenter);
    pointsRed->move(teamsPointsProgess->pos().x() - 100 - 10, 35);
    pointsRed->setStyleSheet("background-color: #ae3838; color: black");
    pointsBlack->adjustSize();
    pointsBlack->resize(100, pointsBlack->size().height());
    pointsBlack->setAlignment(Qt::AlignCenter);
    pointsBlack->move(teamsPointsProgess->pos().x() + teamsPointsProgess->width() + 10, 35);
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

void GameWidget::startGame(int nbPlayers, int nbViews) {

    game = new Game();

    connect(game, &Game::teamsPointsChanged, this, &GameWidget::updateTeamsPoints);
    connect(game, &Game::showEndScreen, this, [=] (int teamWinner) {
        emit setFinishMenuWinner(teamWinner);
        emit setVisibleWidget(3);
    });

    if(nbViews == 0) nbViews = nbPlayers;
    // S'il y a autant de QGraphicsView que de joueurs -> splitscreen
    bool isMultiplayer = nbPlayers == nbViews ? false : true;
    QString terrainFileName = ":/Resources/debugTerrain.tmx";
    game->startGame(terrainFileName, nbPlayers, isMultiplayer, tcpClient);
    viewsLayout = new QHBoxLayout(this);

    for(int i = 0; i < nbViews; i++) {
        View *v = new View(i);
        v->setScene(game);
        viewsLayout->addWidget(v);
        views.append(v);
    }

    viewsLayout->setContentsMargins(0, 0, 0, 0);
    viewsLayout->setSpacing(0);
    setLayout(viewsLayout);
    teamsPointsProgess->raise();
    pointsRed->raise();
    pointsBlack->raise();
    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    ambientMusicPlayer->setMedia(QUrl("qrc:/Resources/sounds/mainTitle.wav"));
    ambientMusicPlayer->play();
    emit stopMenuMusic();

    gameRunning = true;
}

void GameWidget::resetGame() {
    if(!gameRunning) return;
    delete game;
    for(int i = 0; i < views.length(); i++)
        delete views.at(i);
    views.clear();
    delete viewsLayout;
    ambientMusicPlayer->stop();
    gameRunning = false;
}



void GameWidget::updateTeamsPoints(int nbPointsRed, int nbPointsBlack) {
    pointsRed->setText(QString::number(nbPointsRed));
    pointsBlack->setText(QString::number(nbPointsBlack));
    teamsPointsProgess->setMaximum(nbPointsRed + nbPointsBlack);
    teamsPointsProgess->setValue(nbPointsRed);
}
