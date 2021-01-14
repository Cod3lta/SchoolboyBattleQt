#include "gamewidget.h"
#include "stackedwidget.h"
#include "startmenu.h"

StackedWidget::StackedWidget()
{

    GameWidget *gameWidget = new GameWidget(this);
    StartMenu *startMenu = new StartMenu(this);
    QWidget *thirdPageWidget = new QWidget(this);

    addWidget(gameWidget);
    addWidget(startMenu);
    addWidget(thirdPageWidget);

    setCurrentWidget(startMenu);
    connect(startMenu, &StartMenu::changeWidget, this, &StackedWidget::setCurrentIndex);
}
