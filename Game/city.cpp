#include "city.hh"
#include <typeinfo>
#include <stdexcept>
#include <QDebug>

namespace StudentSide
{

City::City():
    gameStatus_(false)
{

}

City::~City()
{

}

void City::setBackground(QImage &basicbackground, QImage &bigbackground)
{    
    if(useSmallMap)
    {
        mainwindow_->setPicture(basicbackground);
        return;
    }
    mainwindow_->setPicture(bigbackground);
}

void City::setClock(QTime clock)
{
    clock_ = clock;
}

void City::addStop(std::shared_ptr<Interface::IStop> stop)
{
    if(typeid(*stop)==typeid(CourseSide::Stop))
    {
        stops_.push_back(stop);
    }
}

void City::startGame()
{
    gameStatus_ = true;
    int passengers = passengers_.size();
    statistics_->morePassengers(passengers);
}

void City::addActor(std::shared_ptr<Interface::IActor> newactor)
{

    if(typeid(*newactor)==typeid(CourseSide::Passenger))
    {
        passengers_.push_back(newactor);
        // to add passengers in mainwindow  while the game is running
        // seuraava if-rakenne varmaankin turha, koska logic ei taida
        // lisätä matkustajia pelin aloittamisen jälkeen ollenkaan
        if(gameStatus_)
        {
            Interface::Location loc = newactor->giveLocation();
            if(loc.giveX() >= 0 and loc.giveX()<=500 and 500-loc.giveY()>=0 and 500-loc.giveY()<=500)
            {
                mainwindow_->addActor(loc.giveX(),500-loc.giveY(),newactor,400);
                qDebug() << "matkustaja lisätty citystä";
            }
        }
    }
    else if(typeid(*newactor)==typeid(CourseSide::Nysse))
    {
        nysses_.push_back(newactor);
        statistics_->newNysse();
        // to add nysses in mainwindow  while the game is running
        if(gameStatus_)
        {
            Interface::Location loc = newactor->giveLocation();
            mainwindow_->addActor(loc.giveX(),500-loc.giveY(),newactor);
        }
    }
    else if(typeid(*newactor)==typeid(Zombie))
    {
        zombies_.push_back(newactor);
        if(gameStatus_)
        {
            Interface::Location loc = newactor->giveLocation();
            mainwindow_->addActor(loc.giveX(),500-loc.giveY(),newactor, 250);
        }
    }
}

void City::removeActor(std::shared_ptr<Interface::IActor> actor)
{
    std::vector<std::shared_ptr<Interface::IActor>>::iterator iterator;

    if(typeid(*actor)==typeid(CourseSide::Passenger))
    {
        for (iterator=passengers_.begin();
             iterator!=passengers_.end();
             iterator++)
        {
            if(actor == *iterator)
            {
                passengers_.erase(iterator);
                actorRemoved(actor);
                return;
            }
        }
    }
    else if(typeid(*actor)==typeid(CourseSide::Nysse))
    {
        for (iterator=nysses_.begin();
             iterator!=nysses_.end();
             iterator++)
        {
            if(actor == *iterator)
            {
                nysses_.erase(iterator);
                statistics_->nysseLeft();
                actorRemoved(actor);
                return;
            }
        }
    }
    else if(typeid(*actor)==typeid(Zombie))
    {
        for (iterator=zombies_.begin();
             iterator!=zombies_.end();
             iterator++)
        {
            if(actor == *iterator)
            {
                zombies_.erase(iterator);
                actorRemoved(actor);
                return;
            }
        }
    }
    throw std::runtime_error("GameError: tried to remove actor that does not exist");
    qDebug() << "GameError: tried to remove actor that does not exist";
}

void City::actorRemoved(std::shared_ptr<Interface::IActor> actor)
{
     if(typeid(*actor)==typeid(CourseSide::Passenger))
     {
         mainwindow_->removePassenger(actor);
         return;
     }
     else if(typeid(*actor)==typeid(CourseSide::Nysse))
     {
         mainwindow_->removeNysse(actor);
         return;
     }
     else if(typeid(*actor)==typeid(Zombie))
     {
         mainwindow_->removeZombie(actor);
         return;
     }
}

bool City::findActor(std::shared_ptr<Interface::IActor> actor) const
{
    if(typeid(*actor)==typeid(CourseSide::Passenger))
    {
        for(auto passenger : passengers_)
        {
            if(passenger == actor)
            {
                return true;
            }
        }
    }
    else if(typeid(*actor)==typeid(CourseSide::Nysse))
    {
        for (auto nysse : nysses_)
        {
            if(nysse == actor)
            {
                return true;
            }
        }
    }
    return false;
}

void City::actorMoved(std::shared_ptr<Interface::IActor> actor)
{
    mainwindow_->moveActor(actor);
}

std::vector<std::shared_ptr<Interface::IActor> > City::getNearbyActors(Interface::Location loc) const
{
    std::vector<std::shared_ptr<Interface::IActor>> nearbyActors;
    for(auto nysse : nysses_)
    {
        if(nysse->giveLocation().isClose(loc))
        {
            nearbyActors.push_back(nysse);
        }
    }
    for(auto passenger : passengers_)
    {
        if(passenger->giveLocation().isClose(loc))
        {
            nearbyActors.push_back(passenger);
        }
    }
    for(auto zombie : zombies_)
    {
        if(zombie->giveLocation().isClose(loc))
        {
            nearbyActors.push_back(zombie);
        }
    }
    return nearbyActors;
}

bool City::isGameOver() const
{
    if (gameStatus_ == false)
    {
        return true;
    }
    else
        return false;
}

std::vector<std::shared_ptr<Interface::IActor>> City::getNysses()
{
    return nysses_;
}

std::vector<std::shared_ptr<Interface::IActor> > City::getPassengers()
{
    return passengers_;
}

std::vector<std::shared_ptr<Interface::IStop> > City::getStops()
{
    return stops_;
}

std::vector<std::shared_ptr<Interface::IActor> > City::getZombies()
{
    return zombies_;
}

void City::takeMainWindow(MainWindow* mainwindow)
{
    mainwindow_ = mainwindow;
}

void City::setStatistics(Statistics* statistics)
{
    statistics_ = statistics;
}

void City::addPlayer(std::shared_ptr<Interface::IActor> player)
{
    player_ = player;

}

std::shared_ptr<Interface::IActor> City::getPlayer()
{
    return player_;
}

void City::addStartingZombie(std::shared_ptr<Interface::IActor> zombie)
{
    zombies_.push_back(zombie);
}


void City::gameOver()
{
    gameStatus_ = false;
}
}
