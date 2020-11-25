#ifndef CITY_HH
#define CITY_HH
#include <map>
#include <vector>
#include "mainwindow.hh"
#include "../Course/CourseLib/interfaces/icity.hh"
#include "../Course/CourseLib/actors/passenger.hh"
#include "../Course/CourseLib/actors/nysse.hh"
#include "../Course/CourseLib/actors/stop.hh"
#include "statistics.hh"
#include "../Course/CourseLib/graphics/simpleactoritem.hh"


namespace StudentSide
{

class City : public Interface::ICity
{

public:
    City();
    virtual ~City();
    virtual void setBackground(QImage& basicbackground, QImage& bigbackground);
    virtual void setClock(QTime clock);
    virtual void addStop(std::shared_ptr<Interface::IStop> stop);
    virtual void startGame();
    virtual void addActor(std::shared_ptr<Interface::IActor> newactor);
    virtual void removeActor(std::shared_ptr<Interface::IActor> actor);
    virtual void actorRemoved(std::shared_ptr<Interface::IActor> actor);
    virtual bool findActor(std::shared_ptr<Interface::IActor> actor) const;
    virtual void actorMoved(std::shared_ptr<Interface::IActor> actor);
    virtual std::vector<std::shared_ptr<Interface::IActor>> getNearbyActors(Interface::Location loc) const;
    virtual bool isGameOver() const;

   std::vector<std::shared_ptr<Interface::IActor>> getNysses();
   std::vector<std::shared_ptr<Interface::IActor>> getPassengers();
   std::vector<std::shared_ptr<Interface::IStop>> getStops();
   std::vector<std::shared_ptr<Interface::IActor>> getZombies();
   void takeMainWindow(MainWindow*);
   void setStatistics(Statistics* statistics);
   void addPlayer(std::shared_ptr<Interface::IActor>);
   std::shared_ptr<Interface::IActor> getPlayer();
   void addStartingZombie(std::shared_ptr<Interface::IActor> zombie);

   void gameOver();


private:

    std::vector<std::shared_ptr<Interface::IActor>> passengers_;
    std::vector<std::shared_ptr<Interface::IActor>> nysses_;
    std::vector<std::shared_ptr<Interface::IStop>> stops_;
    bool gameStatus_;
    QTime clock_;
    MainWindow* mainwindow_;
    std::shared_ptr<Interface::IActor> player_;
    std::vector<std::shared_ptr<Interface::IActor>> zombies_;
    Statistics* statistics_;

    bool useSmallMap = true;
};

}
#endif // CITY_HH
