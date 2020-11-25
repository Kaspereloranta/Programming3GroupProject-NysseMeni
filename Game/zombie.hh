#ifndef ZOMBIE_HH
#define ZOMBIE_HH
#include "../Course/CourseLib/interfaces/iactor.hh"
#include <cstdlib>
#include "../Course/CourseLib/interfaces/icity.hh"

namespace StudentSide
{

class Zombie : public Interface::IActor
{
public:
    Zombie(int hp);
    ~Zombie();

    Interface::Location giveLocation() const;
    bool isRemoved() const;
    void remove();
    void move(Interface::Location loc);
    void setStartingLocation(Interface::Location);
    int getXspeed();
    int getYspeed();
    void changeXspeed(int newXspeed);
    void changeYspeed(int newYspeed);
    void getCity(std::shared_ptr<Interface::ICity> city);
    std::shared_ptr<Interface::ICity> giveCity();
    void takeDamage(int damage);
    int tellHp();
    void addStep();
    void resetSteps();
    int giveSteps();

private:
    Interface::Location location_;
    std::shared_ptr<Interface::ICity> city_;
    bool removed_;


    int hp_;
    int xSpeed_;
    int ySpeed_;
    int stepCounter_;
};
}
#endif // ZOMBIE_HH
