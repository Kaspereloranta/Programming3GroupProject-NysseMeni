#include "zombie.hh"

namespace StudentSide
{


Zombie::Zombie(int hp) :
    hp_(hp),
    stepCounter_(0)
{
    int randNum1 = rand()% 10 + 1;
    int randNum2 = rand()% 2 + 1;
    if (randNum2 == 1) {
        xSpeed_ = randNum1;
    }
    else {
        xSpeed_ = - randNum1;
    }

    int randNum3 = rand()% 10 + 1;
    int randNum4 = rand()% 2 + 1;
    if (randNum4 == 1) {
        ySpeed_ = randNum3;
    }
    else {
        ySpeed_ = - randNum3;
    }
}

Zombie::~Zombie()
{

}

Interface::Location Zombie::giveLocation() const
{
    return location_;
}

bool Zombie::isRemoved() const
{
    return removed_;
}

void Zombie::remove()
{

}

void Zombie::move(Interface::Location loc)
{
    location_ = loc;
}

void Zombie::setStartingLocation(Interface::Location location)
{
    location_ = location;
}

int Zombie::getXspeed()
{
    return xSpeed_;
}

int Zombie::getYspeed()
{
    return ySpeed_;
}

void Zombie::changeXspeed(int newXspeed)
{
    xSpeed_ = newXspeed;
}

void Zombie::changeYspeed(int newYspeed)
{
    ySpeed_ = newYspeed;
}

void Zombie::getCity(std::shared_ptr<Interface::ICity> city)
{
    city_ = city;
}

std::shared_ptr<Interface::ICity> Zombie::giveCity()
{
    return city_;
}

void Zombie::takeDamage(int damage)
{
    hp_ -= damage;
}

int Zombie::tellHp()
{
    return hp_;
}

void Zombie::addStep()
{
    ++stepCounter_;
}

void Zombie::resetSteps()
{
    stepCounter_ = 0;
}

int Zombie::giveSteps()
{
    return stepCounter_;
}
}
