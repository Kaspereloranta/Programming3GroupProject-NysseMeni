#include "player.hh"
#include <QDebug>

namespace StudentSide
{


Player::Player() :
    weapon_("fist")
{

}

Player::~Player()
{

}

Interface::Location Player::giveLocation() const
{
    return location_;
}

bool Player::isRemoved() const
{
    return removed_;
}

void Player::remove()
{

}

void Player::move(Interface::Location loc)
{
    location_ = loc;
}

void Player::setStartingLocation(Interface::Location location)
{
    location_ = location;
}

void Player::getCity(std::shared_ptr<Interface::ICity> city)
{
    city_ = city;
}

std::shared_ptr<Interface::ICity> Player::giveCity()
{
    return city_;
}

QString Player::getWeapon()
{
    return weapon_;
}

void Player::newWeapon(QString weapon)
{
    weapon_ = weapon;
}

}
