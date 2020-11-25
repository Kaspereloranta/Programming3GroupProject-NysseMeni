#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH
#include "../Course/CourseLib/graphics/simpleactoritem.hh"
#include "QMainWindow"
#include "dialog.hh"
#include <utility>
#include "../Course/CourseLib/core/logic.hh"
#include "player.hh"
#include "zombie.hh"
#include <QKeyEvent>
#include <QScrollBar>

namespace Ui {
class MainWindow;
}
namespace StudentSide
{
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setSize(int w, int h);
    void setTick(int t);
    virtual void addActor(int locX, int locY, std::shared_ptr<Interface::IActor> actor, int type = 0);
    void updateCoords(int nX, int nY);
    void setPicture(QImage &img);
    void moveActor(std::shared_ptr<Interface::IActor> actor);

    // dokumentaatioon esiehdot removenysselle ja removepassengerille, saa kutsua vain
    // antamalla tietyn tyyppinen parametri
    void removeNysse(std::shared_ptr<Interface::IActor> nysse);
    void removePassenger(std::shared_ptr<Interface::IActor> passenger);
    void removeZombie(std::shared_ptr<Interface::IActor> zombie);
    void keyPressEvent(QKeyEvent *keyEvent);
    void checkNearbyWeapon(int newX, int newY, QString weapon);

public slots:
    void addStop(int locX, int locY, std::shared_ptr<Interface::IStop> stop, int type = 100);
    void setNameandDifficulty(QString name, int difficulty_level);
    void moveZombies();
    void updatePoints(int points);
    void notifyNewNysse(int nysses);
    void notifyNysseLeaving(int nysses);
    void notifyPassengersAdded(int passengers);
    void updateZombieStats(int zombies, bool didDie);
    void updatePassengersStats(int passengers);
    void scrollTextBrowserDown();
    bool isInGameArea(Interface::Location loc);
    void getNearbyWeapon();
    void updateTimePassed(int seconds, int minutes);
    void endGame(bool didPlayerWin);

signals:
    void gameStarted();
    void sendNameAndDifficulty(QString name, int difficulty_level);
    void gameEnded();
    void passengerLeftScreen();
    void passengerArrivedScreen();
    void passengerToZombie();
    void zombieDied();
    void increasePoints(int points);
    void decreasePoints(int points);

private slots:
    void on_startButton_clicked();

private:

    Ui::MainWindow *ui;
    QGraphicsScene *map;
    QGraphicsScene *weapon;
    QTimer *timer;
    QTimer *statstimer_;
    std::map<std::shared_ptr<Interface::IActor>, std::shared_ptr<CourseSide::SimpleActorItem>> nysses_;
    std::map<std::shared_ptr<Interface::IStop>, std::shared_ptr<CourseSide::SimpleActorItem>> stops_;
    std::map<std::shared_ptr<Interface::IActor>, std::shared_ptr<CourseSide::SimpleActorItem>> passengers_;
    std::map<std::shared_ptr<Interface::IActor>, std::shared_ptr<CourseSide::SimpleActorItem>> player_;
    std::map<std::shared_ptr<Interface::IActor>, std::shared_ptr<CourseSide::SimpleActorItem>> zombies_;

    CourseSide::SimpleActorItem* last_;

    QString name_;
    int difflevel_;
    bool gameStatus_;

    int pointsForKill;
    int minusPointsForDeathPassenger;

    QImage fistImage_;
    QImage swordImage_;
    QImage gunImage_;

    int width_ = 500;
    int height_ = 500;
    int tick_ = 500;

};
}
#endif // MAINWINDOW_HH
