#include <QtTest>
#include "statistics.hh"

// add necessary includes here

class StatisticsTests : public QObject
{
    Q_OBJECT

public:
    StatisticsTests();
    ~StatisticsTests();

private slots:

    void test_giveNysses();
    void test_givePassengers();
    void test_giveZombies();
    void test_givePoints();

};

StatisticsTests::StatisticsTests()
{

}

StatisticsTests::~StatisticsTests()
{

}

void StatisticsTests::test_giveNysses()
{
    StudentSide::Statistics stats;
    int testRounds = rand() % 100+1;
    int index=0;
    while(index <= testRounds)
    {
        stats.newNysse();
        ++index;
    }
    QCOMPARE(index,stats.giveNysses());
    testRounds = testRounds*2;
    while(index <= testRounds)
    {
        stats.newNysse();
        ++index;
    }
    QCOMPARE(index,stats.giveNysses());
    testRounds = testRounds / 2;
    while(index >= testRounds)
    {
        stats.nysseLeft();
        --index;
    }
    QCOMPARE(index,stats.giveNysses());
    while(index >= 0)
    {
        stats.nysseLeft();
        --index;
    }
    QCOMPARE(index,stats.giveNysses());
}

void StatisticsTests::test_givePassengers()
{
    StudentSide::Statistics stats;
    int testRounds = rand() % 100+1;
    int index=0;
    while(index <= testRounds)
    {
        stats.addPassenger();
        ++index;
    }
    QCOMPARE(index,stats.givePassengers());
    testRounds = testRounds*2;
    while(index <= testRounds)
    {
        stats.addPassenger();
        ++index;
    }
    QCOMPARE(index,stats.givePassengers());
    testRounds = testRounds / 2;
    while(index >= testRounds)
    {
        stats.removePassenger();
        --index;
    }
    QCOMPARE(index,stats.givePassengers());
    while(index >= 0)
    {
        stats.removePassenger();
        --index;
    }
    QCOMPARE(index,stats.givePassengers());
    testRounds = rand() % 100+1;
    while(index < testRounds)
    {
        stats.addPassenger();
        ++index;
    }
    int zombies = rand() % 100+1;
    int zombiesCreated = 0;
    index = 0;
    while(index < zombies and zombiesCreated < testRounds)
    {
        stats.passengerTurnedZombie();
        ++index;
        ++zombiesCreated;
    }
    QCOMPARE(testRounds-index,stats.givePassengers());
}

void StatisticsTests::test_giveZombies()
{
    StudentSide::Statistics stats;
    int killZombies = rand() % 11;
    int zombiesKilled = 0;
    while(zombiesKilled < killZombies)
    {
        stats.zombieDied();
        ++zombiesKilled;
    }
    QCOMPARE(10-killZombies,stats.giveZombies());
    int zombiesLeft = 10-killZombies;
    int passengers = rand() % 100;
    int passengersAdded = 0;
    while(passengersAdded < passengers)
    {
        stats.addPassenger();
        ++passengersAdded;
    }
    int passengersTurnedZombie = 0;
    while(passengersTurnedZombie < passengersAdded)
    {
        stats.passengerTurnedZombie();
        ++passengersTurnedZombie;
        ++zombiesLeft;
    }
    QCOMPARE(zombiesLeft,stats.giveZombies());
}

void StatisticsTests::test_givePoints()
{
    StudentSide::Statistics stats;
    unsigned  int pointsToBeAdded = rand() % 1000 + 1;
    stats.increasePoints(pointsToBeAdded);
    QCOMPARE(pointsToBeAdded,stats.givePoints());
    unsigned int pointsToBeDecreased = rand() % 100 + 1;
    stats.decreasePoints(pointsToBeDecreased);
    int points = pointsToBeAdded-pointsToBeDecreased;
    if (points < 0)
    {
        points = 0;
    }
    QCOMPARE(points,stats.givePoints());
}

QTEST_APPLESS_MAIN(StatisticsTests)

#include "tst_statisticstests.moc"
