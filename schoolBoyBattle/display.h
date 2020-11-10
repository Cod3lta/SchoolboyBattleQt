#ifndef DISPLAY_H
#define DISPLAY_H
#include <QLCDNumber>

class Display
{
public:
    Display();

private:
    QLCDNumber score_player_1;
    QLCDNumber score_player_2;
    QLCDNumber timer;

    void displayScore();
    void displayTime();

};

#endif // DISPLAY_H
