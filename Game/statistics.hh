#ifndef STATISTICS_HH
#define STATISTICS_HH
#include <QObject>
#include <QTimer>

namespace StudentSide
{

class Statistics : public QObject
{
    Q_OBJECT
public:
    Statistics(QObject *parent = 0);
    ~Statistics();
    void morePassengers(int num);
    void newNysse();
    void nysseLeft();

    // for unit-testing
    unsigned int giveNysses();
    unsigned int givePassengers();
    unsigned int giveZombies();
    unsigned int givePoints();

public slots:
    void removePassenger();
    void addPassenger();
    void increasePoints(unsigned int points);
    void decreasePoints(unsigned int points);
    void zombieDied();
    void passengerTurnedZombie();
    void checkHasGameEnded();
    void NameAndDiff(QString PlayerName, int difflevel);
    void timer();
    void timeRanOut();

signals:
    void updatePoints(int points);
    void notifyNysseLeaving(int nysses);
    void nysseArrived(int nysses);
    void updatePassengers(int passengers);
    void updateZombies(int zombies, bool didDie);
    void updateTimePassed(int seconds, int minutes);
    void allPassengers(int passengers);
    void endGame(bool didPlayerWin);

private:

    int difficulty_;
    QString playerName_;
    unsigned int nysses_;
    unsigned int passengers_;
    unsigned int zombies_;
    unsigned int points_;
    QTimer* gameTimer_;
    QTimer* stopWatch_;
    int gameTime_;
    int secondsPassed_;
    int minutesPassed_;
};
}
#endif // STATISTICS_HH
