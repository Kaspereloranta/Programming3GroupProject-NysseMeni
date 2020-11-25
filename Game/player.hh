#ifndef PLAYER_HH
#define PLAYER_HH

#include "../Course/CourseLib/interfaces/iactor.hh"
#include "../Course/CourseLib/interfaces/icity.hh"


namespace StudentSide
{


class Player : public Interface::IActor
{
public:
    Player();
    ~Player();


    Interface::Location giveLocation() const;
    bool isRemoved() const;
    void remove();
    void move(Interface::Location loc);
    void setStartingLocation(Interface::Location);
    void getCity(std::shared_ptr<Interface::ICity> city);
    std::shared_ptr<Interface::ICity> giveCity();
    QString getWeapon();
    void newWeapon(QString weapon);


private:
    Interface::Location location_;
    std::shared_ptr<Interface::ICity> city_;
    bool removed_;


    QString weapon_;
};
}
#endif // PLAYER_HH
