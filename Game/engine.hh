#ifndef ENGINE_HH
#define ENGINE_HH

#include "../Course/CourseLib/core/logic.hh"
#include "../Course/CourseLib/core/location.hh"
#include "city.hh"
#include "mainwindow.hh"
#include "player.hh"
#include "zombie.hh"
#include "dialog.hh"
#include "statistics.hh"
#include <QApplication>


namespace StudentSide
{

class Engine : public QObject
{
    Q_OBJECT
public:
    Engine(int argc, char *argv[], QObject *parent = 0);
    int initializeGame(int argc, char *argv[]);
signals:
    void drawActor(int locX, int locY, std::shared_ptr<Interface::IActor> actor, int type = 0);
    void drawStop (int locX, int locY, std::shared_ptr<Interface::IStop> actor, int type = 100);
    void drawPlayer (int locX, int locY, std::shared_ptr<Interface::IActor> actor, int type = 200);
    void drawZombie (int locX, int locY, std::shared_ptr<Interface::IActor> actor, int type = 250);
public slots:
    void startGame();
    void setNameAndDifficulty(QString name, int difficultyLevel);
    void endGame();

private:
    CourseSide::Logic logic_;
    std::shared_ptr<City> city_;
    QString playerName_;
    int difficultyLevel_;
};



}
#endif // ENGINE_HH

