#include "engine.hh"
#include "mainwindow.hh"
#include <QDebug>

namespace StudentSide
{

const QString & DEFAULT_SMALL_MAP_FILE = ":/offlinedata/offlinedata/kartta_pieni_500x500.png";
const QString & DEFAULT_LARGE_MAP_FILE = ":/offlinedata/offlinedata/kartta_iso_1095x592.png";

Engine::Engine(int argc, char *argv[], QObject *parent ):
    QObject(parent)
{    
    initializeGame(argc,argv);
}

int Engine::initializeGame(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow m;
    MainWindow* m_pointer = &m;
    connect(m_pointer, &MainWindow::sendNameAndDifficulty,this,&Engine::setNameAndDifficulty);
    connect(m_pointer, &MainWindow::gameStarted,this,&Engine::startGame);
    connect(m_pointer, &MainWindow::gameEnded,this,&Engine::endGame);
    connect(this,&Engine::drawActor,m_pointer,&MainWindow::addActor);
    connect(this,&Engine::drawStop,m_pointer,&MainWindow::addStop);
    connect(this,&Engine::drawPlayer,m_pointer,&MainWindow::addActor);
    connect(this,&Engine::drawZombie,m_pointer,&MainWindow::addActor);
    m.show();

    logic_.fileConfig();
    QTime time = QTime(time.currentTime());
    unsigned short int hour = time.hour();
    unsigned short int min = time.minute();
    logic_.setTime(hour,min);
    City cityobj;
    std::shared_ptr<City> city = std::make_shared<City>(cityobj);
    Statistics stats;
    Statistics* statistics = &stats;
    connect(m_pointer, &MainWindow::sendNameAndDifficulty,statistics,&Statistics::NameAndDiff);
    connect(m_pointer, &MainWindow::passengerLeftScreen,statistics,&Statistics::removePassenger);
    connect(m_pointer, &MainWindow::passengerArrivedScreen,statistics,&Statistics::addPassenger);
    connect(m_pointer, &MainWindow::passengerToZombie,statistics,&Statistics::passengerTurnedZombie);
    connect(m_pointer, &MainWindow::zombieDied,statistics,&Statistics::zombieDied);
    connect(m_pointer, &MainWindow::increasePoints,statistics,&Statistics::increasePoints);
    connect(m_pointer, &MainWindow::decreasePoints,statistics,&Statistics::decreasePoints);
    connect(statistics, &Statistics::notifyNysseLeaving, m_pointer, &MainWindow::notifyNysseLeaving);
    connect(statistics, &Statistics::nysseArrived, m_pointer, &MainWindow::notifyNewNysse);
    connect(statistics, &Statistics::updatePassengers, m_pointer, &MainWindow::updatePassengersStats);
    connect(statistics, &Statistics::updateZombies, m_pointer, &MainWindow::updateZombieStats);
    connect(statistics, &Statistics::updatePoints, m_pointer, &MainWindow::updatePoints);
    connect(statistics, &Statistics::allPassengers, m_pointer, &MainWindow::notifyPassengersAdded);
    connect(statistics, &Statistics::updateTimePassed, m_pointer, &MainWindow::updateTimePassed);
    connect(statistics, &Statistics::endGame, m_pointer, &MainWindow::endGame);

    city_ = city;
    city_->takeMainWindow(m_pointer);
    city_->setStatistics(statistics);

    std::shared_ptr<Player> player = std::make_shared<Player>();
    // pelaajan locationin arvot valittu niin, että aloituspaikan x-koordinaatti on 0 ja
    // y-koordinaatti 10, jolloin pelaaja näkyy kokonaan kartalla
    player->setStartingLocation(Interface::Location(6825333,3328295));
    player->getCity(city_);
    city_->addPlayer(player);

    logic_.takeCity(city);

    QImage smallMap(DEFAULT_SMALL_MAP_FILE);
    QImage largeMap(DEFAULT_LARGE_MAP_FILE);
    city_->setBackground(smallMap,largeMap);

    return a.exec();
}

void Engine::startGame()
{
    logic_.finalizeGameStart();

    qDebug() << "Peli alkaa, " << playerName_ << "!";
    qDebug() << "Valitsemasi vaikeustaso on" << difficultyLevel_;

    for(auto nysse : city_->getNysses())
    {
        Interface::Location loc = nysse->giveLocation();
        emit drawActor(loc.giveX(),500-loc.giveY(),nysse);
    }
    for (auto passenger : city_->getPassengers())
    {
        Interface::Location loc = passenger->giveLocation();
        if(loc.giveX() >= 0 and loc.giveX()<=500 and 500-loc.giveY()>=0 and 500-loc.giveY()<=500)
        {
            emit drawActor(loc.giveX(),500-loc.giveY(),passenger,400);
        }
    }
    Interface::Location loc = city_->getPlayer()->giveLocation();
    emit drawPlayer(loc.giveX(),500-loc.giveY(),city_->getPlayer());


    for (int i = 0; i < 10; ++i) {
        std::shared_ptr<Zombie> zombie = std::make_shared<Zombie>(2 * difficultyLevel_);
        zombie->setStartingLocation(Interface::Location(6824823,3327845));
        zombie->getCity(city_);
        city_->addStartingZombie(zombie);
    }

    for(auto zombie : city_->getZombies())
    {
        Interface::Location loc = zombie->giveLocation();
        emit drawZombie(loc.giveX(),500-loc.giveY(),zombie);
    }

}

void Engine::setNameAndDifficulty(QString name, int difficultyLevel)
{
    playerName_ = name;
    difficultyLevel_ = difficultyLevel;
}

void Engine::endGame()
{
    city_->gameOver();
}

}

