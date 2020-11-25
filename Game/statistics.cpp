#include "statistics.hh"
#include <QDebug>
#include <iostream>
#include <fstream>

namespace StudentSide
{

Statistics::Statistics(QObject *parent):
    QObject(parent),
    nysses_(0),
    passengers_(0),
    zombies_(10),
    points_(0),
    gameTime_(300000),
    secondsPassed_(0),
    minutesPassed_(0)
{
    gameTimer_ = new QTimer(this);
    stopWatch_ = new QTimer(this);
    connect(gameTimer_,&QTimer::timeout,this,&Statistics::timeRanOut);
    connect(stopWatch_,&QTimer::timeout,this,&Statistics::timer);
}

Statistics::~Statistics()
{

}

void Statistics::morePassengers(int num)
{
    emit allPassengers(num);
    // to setup zombiestats
    // in mainwindow
    emit updateZombies(zombies_,false);
    //becuase this function  is
    // called in city when the
    // game start
    gameTimer_->start(gameTime_);
    stopWatch_->start(1000);
}

void Statistics::newNysse()
{
    ++nysses_;
    emit nysseArrived(nysses_);
}

void Statistics::nysseLeft()
{
    --nysses_;
    emit notifyNysseLeaving(nysses_);
}

void Statistics::increasePoints(unsigned int points)
{
    points_ += points;
    emit updatePoints(points_);
}

void Statistics::decreasePoints(unsigned int points)
{
    if(points_ <= points)
    {
        points_ = 0;
    }
    else
    {
        points_ -= points;
    }
    emit updatePoints(points_);
}

void Statistics::zombieDied()
{
    --zombies_;
    emit updateZombies(zombies_,true);
    checkHasGameEnded();
}

void Statistics::passengerTurnedZombie()
{
    --passengers_;
    ++zombies_;
    emit updateZombies(zombies_,false);
    emit updatePassengers(passengers_);
}

void Statistics::checkHasGameEnded()
{
    if(zombies_==0)
    {
        if(difficulty_==1)
        {
            std::ofstream dataFile;
            dataFile.open("TOP-10-Data-Level-1.txt", std::ios_base::app);
            std::string name = playerName_.toUtf8().constData();
            dataFile << name << " " << points_<< "\n";
            dataFile.close();
            stopWatch_->stop();
            gameTimer_->stop();
            emit endGame(true);
            return;
        }
        else if(difficulty_==2)
        {
            std::ofstream dataFile;
            dataFile.open("TOP-10-Data-Level-2.txt", std::ios_base::app);
            std::string name = playerName_.toUtf8().constData();
            dataFile << name << " " << points_<< "\n";
            dataFile.close();
            emit endGame(true);
            stopWatch_->stop();
            gameTimer_->stop();
            return;
        }
        std::ofstream dataFile;
        dataFile.open("TOP-10-Data-Level-3.txt", std::ios_base::app);
        std::string name = playerName_.toUtf8().constData();
        dataFile << name << " " << points_<< "\n";
        dataFile.close();
        stopWatch_->stop();
        gameTimer_->stop();
        emit endGame(true);
    }
}

void Statistics::NameAndDiff(QString PlayerName, int difflevel)
{
    difficulty_ = difflevel;
    playerName_ = PlayerName;
}

void Statistics::timer()
{
    ++secondsPassed_;
    if(secondsPassed_== 60)
    {
        secondsPassed_ = 0;
        ++minutesPassed_;
    }
    emit updateTimePassed(secondsPassed_,minutesPassed_);
}

void Statistics::timeRanOut()
{
    emit endGame(false);
    // to set the mainwindow
    // show the actual time spent,
    // if timer() were not called here,
    // mainwindow's time passed would
    // lack one second
    timer();
    qDebug() << gameTimer_->remainingTime();
    stopWatch_->stop();
    gameTimer_->stop();
}

unsigned int Statistics::giveNysses()
{
    return nysses_;
}

unsigned int Statistics::givePassengers()
{
    return passengers_;
}

unsigned int Statistics::giveZombies()
{
    return zombies_;
}

unsigned int Statistics::givePoints()
{
    return points_;
}

void Statistics::removePassenger()
{
    --passengers_;
    emit updatePassengers(passengers_);
}

void Statistics::addPassenger()
{
    ++passengers_;
    emit updatePassengers(passengers_);
}

}
