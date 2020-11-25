#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <QDebug>

namespace StudentSide
{

const QString & GUN_IMAGE = ":/images/gun_image.png";
const QString & SWORD_IMAGE = ":/images/sword_image.png";
const QString & FIST_IMAGE = ":/images/fist_image.png";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    name_("DefaultPlayer"),
    difflevel_(1),
    gameStatus_(false),
    pointsForKill(1),
    minusPointsForDeathPassenger(1),
    fistImage_(QImage(FIST_IMAGE)),
    swordImage_(QImage(SWORD_IMAGE)),
    gunImage_(QImage(GUN_IMAGE))
{
    ui->setupUi(this);
    map = new QGraphicsScene(this);
    ui->graphicsView->setScene(map);
    map->setSceneRect(0,0,width_,height_);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, map, &QGraphicsScene::advance);
    timer->start(tick_);

    Dialog d;
    Dialog* d_pointer = &d;
    connect(d_pointer, &Dialog::player_name_and_difficulty,this,&MainWindow::setNameandDifficulty);
    d.exec();

    QPalette pal = ui->weaponButton->palette();
    pal.setColor(QPalette::Button,QColor(Qt::red));
    ui->weaponButton->setAutoFillBackground(true);
    ui->weaponButton->setPalette(pal);
    ui->weaponButton->update();
    ui->weaponButton->setEnabled(false);

    connect(timer, &QTimer::timeout, this, &MainWindow::moveZombies);
    connect(ui->weaponButton, &QPushButton::clicked, this, &MainWindow::getNearbyWeapon);

    weapon = new QGraphicsScene(this);
    ui->weaponInUse->setScene(weapon);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setSize(int w, int h)
{
    width_ = w;
    height_ = h;
}

void MainWindow::setTick(int t)
{
    tick_ = t;
}

void MainWindow::addActor(int locX, int locY, std::shared_ptr<Interface::IActor> actor, int type )
{
    if (typeid(*actor)==typeid(CourseSide::Nysse)) {
        std::shared_ptr<CourseSide::SimpleActorItem> graphicActor =
                std::make_shared<CourseSide::SimpleActorItem>(locX,locY,type);
        nysses_.insert({actor,graphicActor});
        map->addItem(graphicActor.get());
        last_ = graphicActor.get();
    }
    else if (typeid(*actor)==typeid(CourseSide::Passenger)) {
        std::shared_ptr<CourseSide::SimpleActorItem> graphicActor =
                std::make_shared<CourseSide::SimpleActorItem>(locX,locY,type);
        CourseSide::Passenger* psngr = dynamic_cast<CourseSide::Passenger*>(actor.get());
        if(psngr!=0 and psngr->isInVehicle()==true)
        {
            graphicActor.get()->setVisible(false);
        }
        passengers_.insert({actor,graphicActor});
        map->addItem(graphicActor.get());
        last_ = graphicActor.get();
        emit passengerArrivedScreen();
    }
    else if (typeid(*actor)==typeid(StudentSide::Player)) {
        std::shared_ptr<CourseSide::SimpleActorItem> graphicActor =
                std::make_shared<CourseSide::SimpleActorItem>(locX,locY,type);
        player_.insert({actor,graphicActor});
        map->addItem(graphicActor.get());
        last_ = graphicActor.get();
    }
    else if (typeid(*actor)==typeid(StudentSide::Zombie)) {
        std::shared_ptr<CourseSide::SimpleActorItem> graphicActor =
                std::make_shared<CourseSide::SimpleActorItem>(locX,locY,type);
        zombies_.insert({actor,graphicActor});
        map->addItem(graphicActor.get());
        last_ = graphicActor.get();
    }
}

void MainWindow::addStop(int locX, int locY, std::shared_ptr<Interface::IStop> stop, int type)
{
    std::shared_ptr<CourseSide::SimpleActorItem> graphicStop =
            std::make_shared<CourseSide::SimpleActorItem>(locX,locY,type);
    stops_.insert({stop,graphicStop});
    map->addItem(graphicStop.get());
    last_ = graphicStop.get();
}

void MainWindow::updateCoords(int nX, int nY)
{
    last_->setCoord(nX, nY);
}

void MainWindow::setPicture(QImage &img)
{
    map->setBackgroundBrush(img);
}

void MainWindow::setNameandDifficulty(QString name, int difficulty_level)
{
    name_ = name;
    difflevel_ = difficulty_level;   
    if(difflevel_ == 2)
    {
        minusPointsForDeathPassenger = 2;
    }
    else if(difflevel_ == 3)
    {
        minusPointsForDeathPassenger = 3;
    }
}

void MainWindow::moveZombies()
{
    if (gameStatus_ == false) {
        return;
    }

    std::map<std::shared_ptr<Interface::IActor>, std::shared_ptr<CourseSide::SimpleActorItem>>::iterator iterator;

    for (iterator=zombies_.begin();
         iterator!=zombies_.end();
         iterator++)
    {
        Interface::Location loc = iterator->first->giveLocation();
        Zombie* zombie = dynamic_cast<Zombie*>(iterator->first.get());

        int currentX = loc.giveX();
        int currentY = loc.giveY();
        int newX = currentX + zombie->getXspeed();
        int newY = currentY + zombie->getYspeed();

        if (newX < 0) {
            int newXspeed = rand()% 10 + 1;
            zombie->changeXspeed(newXspeed);
            newX = currentX;
        }
        else if (newX > 490) {
            int newXspeed = - rand()% 10 + 1;
            zombie->changeXspeed(newXspeed);
            newX = currentX;
        }
        if (newY < 10) {
            int newYspeed = rand()% 10 + 1;
            zombie->changeYspeed(newYspeed);
            newY = currentY;
        }
        else if (newY > 500) {
            int newYspeed = - rand()% 10 + 1;
            zombie->changeYspeed(newYspeed);
            newY = currentY;
        }

        loc.setXY(newX, newY);
        iterator->first->move(loc);
        iterator->second->setCoord(loc.giveX(),500-loc.giveY());

        Interface::Location newLoc = zombie->giveLocation();
        std::vector<std::shared_ptr<Interface::IActor>> nearbyActors = zombie->giveCity()->getNearbyActors(loc);
        for (auto actor : nearbyActors) {
            if(typeid(*actor)==typeid(CourseSide::Passenger)) {
                CourseSide::Passenger* passenger = dynamic_cast<CourseSide::Passenger*>(actor.get());
                if (zombie->giveSteps() > 8) {
                    if (passenger->isInVehicle() == false) {
                        zombie->giveCity()->removeActor(actor);
                        std::shared_ptr<Zombie> newZombie = std::make_shared<Zombie>(2 * difflevel_);
                        newZombie->setStartingLocation(newLoc);
                        newZombie->getCity(zombie->giveCity());
                        zombie->giveCity()->addActor(newZombie);
                        zombie->resetSteps();
                        emit passengerToZombie();
                        emit decreasePoints(minusPointsForDeathPassenger);
                    }
                }

            }

        }
        zombie->addStep();
    }
}


void MainWindow::moveActor(std::shared_ptr<Interface::IActor> actor)
{
    Interface::Location loc = actor->giveLocation();
    if (typeid(*actor)==typeid(CourseSide::Nysse))
    {
        for( auto nysse : nysses_)
        {
            if(nysse.first == actor)
            {
                nysses_.at(actor)->setCoord(loc.giveX(),500-loc.giveY());
                return;
            }
        }
    }

    else if (typeid(*actor)==typeid(CourseSide::Passenger))
    {
        for( auto passenger : passengers_)
        {
            if(passenger.first == actor)
            {
                CourseSide::Passenger* psngr = dynamic_cast<CourseSide::Passenger*>(passenger.first.get());
                if(isInGameArea(loc))
                {
                    passengers_.at(actor)->setCoord(loc.giveX(),500-loc.giveY());
                    if(psngr->isInVehicle())
                    {
                        passengers_.at(actor)->setVisible(false);
                        return;
                    }
                    passengers_.at(actor)->setVisible(true);
                    return;
                }
                // to remove passengers who moved outside the screen
                removePassenger(actor);
                emit passengerLeftScreen();
                return;
            }
        }
        // to add passengers who were not on the screen area at
        // the beginnning of the game and now moved to the
        // game area
        if( isInGameArea(loc))
        {
            addActor(loc.giveX(),loc.giveY(),actor,400);
        }
    }
}

void MainWindow::removeNysse(std::shared_ptr<Interface::IActor> nysse)
{
    nysses_.erase(nysse);
}

void MainWindow::removePassenger(std::shared_ptr<Interface::IActor> passenger)
{
    passengers_.erase(passenger);
}

void MainWindow::removeZombie(std::shared_ptr<Interface::IActor> zombie)
{
    zombies_.erase(zombie);
}


void MainWindow::keyPressEvent(QKeyEvent *keyEvent)
{
    if (gameStatus_ == false) {
        return;
    }

    Interface::Location loc = player_.begin()->first->giveLocation();
    Player* player = dynamic_cast<Player*>(player_.begin()->first.get());

    int currentX = loc.giveX();
    int currentY = loc.giveY();

    if (keyEvent->key() == Qt::Key_A) {

        int newX = currentX - 10;
        //qDebug() << newX;

        if (newX >= 0) {
            loc.setXY(newX, currentY);
            player_.begin()->first->move(loc);
            player_.begin()->second->setCoord(loc.giveX(),500-loc.giveY());
            checkNearbyWeapon(newX, currentY, player->getWeapon());
        }
        else {
            return;
        }
    }

    else if (keyEvent->key() == Qt::Key_D) {

        int newX = currentX + 10;
        //qDebug() << newX;

        // 490 on suurin x-koordinaatti, jolloin pelaaja on vielä kokonaan kartalla
        if (newX <= 490) {
            loc.setXY(newX, currentY);
            player_.begin()->first->move(loc);
            player_.begin()->second->setCoord(loc.giveX(),500-loc.giveY());
            checkNearbyWeapon(newX, currentY, player->getWeapon());
        }
        else {
            return;
        }
    }

    else if (keyEvent->key() == Qt::Key_W) {

        int newY = currentY + 10;
        //qDebug() << newY;

        if (newY <= 500) {
            loc.setXY(currentX, newY);
            player_.begin()->first->move(loc);
            player_.begin()->second->setCoord(loc.giveX(),500-loc.giveY());
            checkNearbyWeapon(currentX, newY, player->getWeapon());
        }
        else {
            return;
        }
    }

    else if (keyEvent->key() == Qt::Key_S) {

        int newY = currentY - 10;
        //qDebug() << newY;

        // 10 on pienin y-koordinaatti, jolloin pelaaja on vielä kokonaan kartalla
        if (newY >= 10) {
            loc.setXY(currentX, newY);
            player_.begin()->first->move(loc);
            player_.begin()->second->setCoord(loc.giveX(),500-loc.giveY());
            checkNearbyWeapon(currentX, newY, player->getWeapon());
        }
        else {
            return;
        }
    }

    else if (keyEvent->key() == Qt::Key_E) {

        QString weapon = player->getWeapon();

        if (weapon == "fist") {
            std::vector<std::shared_ptr<Interface::IActor>> nearbyActors = player->giveCity()->getNearbyActors(loc);
            for (auto actor : nearbyActors) {
                if(typeid(*actor)==typeid(Zombie)) {
                    Zombie* zombie = dynamic_cast<Zombie*>(actor.get());
                    if (zombie->tellHp() == 1) {
                        player->giveCity()->removeActor(actor);
                        emit increasePoints(pointsForKill);
                        emit zombieDied();
                    }
                    else {
                        zombie->takeDamage(1);
                    }
                }
            }
        }

        if (weapon == "sword") {
            std::vector<std::shared_ptr<Interface::IActor>> nearbyActors = player->giveCity()->getNearbyActors(loc);
            for (auto actor : nearbyActors) {
                if(typeid(*actor)==typeid(Zombie)) {
                    Zombie* zombie = dynamic_cast<Zombie*>(actor.get());
                    if (zombie->tellHp() <= 2) {
                        player->giveCity()->removeActor(actor);
                        emit increasePoints(pointsForKill);
                        emit zombieDied();
                    }
                    else {
                        zombie->takeDamage(2);
                    }
                }
            }
        }

        if (weapon == "gun") {
            std::map<std::shared_ptr<Interface::IActor>, std::shared_ptr<CourseSide::SimpleActorItem>>::iterator iterator;
            std::vector<std::shared_ptr<Interface::IActor>> nearbyZombies;

            for (iterator=zombies_.begin();
                 iterator!=zombies_.end();
                 iterator++)
            {
                if(iterator->first->giveLocation().isClose(loc, 30))
                {
                    nearbyZombies.push_back(iterator->first);
                }
            }
            for (auto actor : nearbyZombies) {
                if(typeid(*actor)==typeid(Zombie)) {
                    Zombie* zombie = dynamic_cast<Zombie*>(actor.get());
                    if (zombie->tellHp() <= 2) {
                        player->giveCity()->removeActor(actor);
                        emit increasePoints(pointsForKill);
                        emit zombieDied();
                    }
                    else {
                        zombie->takeDamage(2);
                    }
                }
            }
        }
    }
}

void MainWindow::checkNearbyWeapon(int x, int y, QString weapon)
{
    if ( 290 <= x && x <= 310 && 310 <= y && y <= 330) {
        if (weapon != "sword") {
            qDebug() << "sword is nearby";
            QPalette pal = ui->weaponButton->palette();
            pal.setColor(QPalette::Button,QColor(Qt::green));
            ui->weaponButton->setAutoFillBackground(true);
            ui->weaponButton->setPalette(pal);
            ui->weaponButton->setEnabled(true);
            ui->weaponButton->update();
        }
    }
    else if ( 240 <= x && x <= 260 && 120 <= y && y <= 140) {
        if (weapon != "gun") {
            qDebug() << "gun is nearby";
            QPalette pal = ui->weaponButton->palette();
            pal.setColor(QPalette::Button,QColor(Qt::green));
            ui->weaponButton->setAutoFillBackground(true);
            ui->weaponButton->setPalette(pal);
            ui->weaponButton->setEnabled(true);
            ui->weaponButton->update();
        }
    }
    else {
        QPalette pal = ui->weaponButton->palette();
        pal.setColor(QPalette::Button,QColor(Qt::red));
        ui->weaponButton->setAutoFillBackground(true);
        ui->weaponButton->setPalette(pal);
        ui->weaponButton->update();
        ui->weaponButton->setEnabled(false);
    }
}



void MainWindow::updatePoints(int points)
{
    ui->lcdPoints->display(points);
}

void MainWindow::notifyNewNysse(int nysses)
{
    ui->statisticsWindow1->insertPlainText("Nysse added!\n");
    ui->lcdNysses->display(nysses);
    scrollTextBrowserDown();
}

void MainWindow::notifyNysseLeaving(int nysses)
{
    ui->lcdNysses->display(nysses);
    ui->statisticsWindow1->insertPlainText("Nysse left the game!\n");
    scrollTextBrowserDown();
}

void MainWindow::notifyPassengersAdded(int passengers)
{
    QString p = p.number(passengers);
    ui->statisticsWindow1->insertPlainText(p + " passengers added to the"
                                               " city of Tampere!\n");
    scrollTextBrowserDown();
}

void MainWindow::updateZombieStats(int zombies,  bool didDie)
{
    ui->lcdZombies->display(zombies);
    if(didDie)
    {
        ui->statisticsWindow1->insertPlainText("Zombie died! \n");
        scrollTextBrowserDown();
        return;
    }
    if(gameStatus_)
    {
        ui->statisticsWindow1->insertPlainText("Passenger died! \n");
        scrollTextBrowserDown();
    }
}

void MainWindow::updatePassengersStats(int passengers)
{
    ui->lcdPassengers->display(passengers);
}

void MainWindow::scrollTextBrowserDown()
{
    QScrollBar *sb = ui->statisticsWindow1->verticalScrollBar();
    sb->setValue(sb->maximum());
}

bool MainWindow::isInGameArea(Interface::Location loc)
{
    if(loc.giveX() >= 0 and loc.giveX()<=500 and 500-loc.giveY()>=0 and 500-loc.giveY()<=500)
    {
        return true;
    }
    return false;
}

void MainWindow::getNearbyWeapon()
{
    Interface::Location loc = player_.begin()->first->giveLocation();
    Player* player = dynamic_cast<Player*>(player_.begin()->first.get());

    if ( 290 <= loc.giveX() && loc.giveX() <= 310 && 310 <= loc.giveY() && loc.giveY() <= 330) {
        player->newWeapon("sword");
        ui->weaponInUse->setBackgroundBrush(swordImage_);
        weapon->setSceneRect(0,0,59,30);
        qDebug() << "You found a sword";

    }
    else if ( 240 <= loc.giveX() && loc.giveX() <= 260 && 120 <= loc.giveY() && loc.giveY() <= 140) {
        player->newWeapon("gun");
        ui->weaponInUse->setBackgroundBrush(gunImage_);
        weapon->setSceneRect(0,0,59,37);
        qDebug() << "You found a gun";
    }
    QPalette pal = ui->weaponButton->palette();
    pal.setColor(QPalette::Button,QColor(Qt::red));
    ui->weaponButton->setAutoFillBackground(true);
    ui->weaponButton->setPalette(pal);
    ui->weaponButton->update();
    ui->weaponButton->setEnabled(false);

}

void MainWindow::updateTimePassed(int seconds, int minutes)
{
    ui->lcdSeconds->display(seconds);
    ui->lcdMinutes->display(minutes);
}

void MainWindow::endGame(bool didPlayerWin)
{
    if(didPlayerWin)
    {
        ui->statisticsWindow1->insertPlainText("\nCongratulations, you won the game! "
                                               "You killed every zombie and saved the"
                                               " city of Tampere!\n\nPlease"
                                               " press Quit.");
        scrollTextBrowserDown();
    }
    else
    {
        ui->statisticsWindow1->insertPlainText("\nTime ran out, zombies have taken over"
                                               " the city of Tamper. You lost the game!"
                                               "\n\nPlease press Quit.");
        scrollTextBrowserDown();
    }
    emit gameEnded();
    gameStatus_ = false;
}

void MainWindow::on_startButton_clicked()
{
    qDebug() << "Start clicked";
    emit sendNameAndDifficulty(name_, difflevel_);
    emit gameStarted();
    weapon->setSceneRect(0,0,45,35);
    ui->weaponInUse->setBackgroundBrush(fistImage_);
    ui->startButton->setEnabled(false);
    gameStatus_ = true;
}
}

