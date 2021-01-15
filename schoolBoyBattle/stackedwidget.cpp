#include "gamewidget.h"
#include "stackedwidget.h"
#include "startmenu.h"

StackedWidget::StackedWidget()
{

    gameWidget = new GameWidget(this);
    StartMenu *startMenu = new StartMenu(this);
    QWidget *thirdPageWidget = new QWidget(this);

    addWidget(gameWidget);
    addWidget(startMenu);
    addWidget(thirdPageWidget);

    setCurrentWidget(startMenu);
    connect(startMenu, &StartMenu::startLocalGame, gameWidget, &GameWidget::restartLocalGame);
    connect(startMenu, &StartMenu::setVisibleWidget, this, &QStackedWidget::setCurrentIndex);
    setFocusPolicy(Qt::StrongFocus);
}
