# SchoolBoyBattle
SchoolBoyBattle is a game made in Qt using the QGraphicsFramework. The goal of the game is to capture as much candy as possible in the school and in the yard and bring it back to St-Nicolas or Père Fouettard depending on the team you are in !

![Imgur](https://i.imgur.com/mfz3cjA.png)

you can also steal other players' candy, and the bigger the candy, the more points it will earn when it is brought back to the team.

![Imgur](https://i.imgur.com/WsyzJQ7.gif) ![Imgur](https://i.imgur.com/fEikWPJ.gif)

The game features original graphics and musics, a local coop game with 2 players and a multiplayer game from 2 to 8 players.

![Imgur](https://imgur.com/svc2tQr.gif) ![Imgur](https://imgur.com/lSpWadQ.gif) ![Imgur](https://imgur.com/q8UxTeR.gif) ![Imgur](https://imgur.com/8UNYIO0.gif)

## Notes

If you are interested to see how to create a network app, take a look at this example I used to create this game: https://github.com/VSRonin/ChatExample

- Currently, the multiplayer is badly implemented (every client is the "master" of the current player and its candies) causing bugs and synchronisations problems. Don't copy what I did, instead create a server with an instance where everything in the game is synchronized, and the clients depends on this instance.
- Actually, when I think about it, the project is poorly done at some places... The development was a bit rushed as we had to finish it in under a month, but anyway I'm still proud of the result :)


## Simplified UML diagram

![Imgur](https://i.imgur.com/8nuh7cl.png)

| Color | Deriviated class |
| ------- | -------------- |
| ![](https://i.imgur.com/YZkMM7O.png) | `QStackedWidget` |
| ![](https://i.imgur.com/4UiPqXj.png) | `QWidget` |
| ![](https://i.imgur.com/mfgMQ2R.png) | `QObject` |
| ![](https://i.imgur.com/ikoMsr8.png) | `QGraphicsView` |
| ![](https://i.imgur.com/nMMxoDM.png) | `QGraphicsScene` |
| ![](https://i.imgur.com/VNb5qmk.png) | `QGraphicsItem` |
| ![](https://i.imgur.com/0lY3uqP.png) | This class isn't deriviated from another Qt class |


| Connection | Explanation |
| ------- | -------------- |
| ![](https://i.imgur.com/FTDrlSH.png) | The object of type `A` <br>creates an object of type `B` |
| ![](https://i.imgur.com/NiQ9e63.png) | The object of type `A` <br>has a member <br>pointing to the object <br>of type `B` |
| ![](https://i.imgur.com/5b3un4l.png) | The class `A` deriviates <br> from the class `B` |
