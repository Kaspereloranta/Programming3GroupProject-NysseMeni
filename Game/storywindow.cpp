#include "storywindow.hh"
#include "ui_storywindow.h"
#include <QScrollBar>

namespace StudentSide
{
const QString lore1 = " It's year 2050 and the mankind have to face"
                      " a greater challenge it ever has.";
const QString lore2 = " The oil is running out form the planet Earth"
                      " which is why governmentes around the world"
                      " have forbidden individuals to own cars.";
const QString lore3 = " Because electric cars never reached their"
                      " full potential and companies trying to develop"
                      " them faced bankruptcies, people have to"
                      " use public transport to move around. \n";
const QString lore4 = "\nJust after when healt authorities managed"
                      " to isolate the Coronavirus, it turned out that"
                      " the virus had evolved to whole new form,"
                      " which is significantly more dangerous than"
                      " the previous virus.";
const QString lore5 = " Since this is a new form of a"
                      " virus, health authorities were not paying attention"
                      " to isolate it, and the new virus is spreading"
                      " faster than any virus before.";
const QString lore6 = " The new virus corrodes the human brain in a"
                      " matter of seconds, and makes humans extremely"
                      " aggressive and violent.";
const QString lore7 = " Healt authorities call infected people zombies,"
                      " since they act exactly the same way than those zombies"
                      " we have used to seen on movies and video games.";
const QString lore8 = " The virus is spreading worldwide extremely fast due"
                      " to zombies who can infect other people by biting them. \n";
const QString lore9 = " \nToday, the virus is reaching city of Tampere in Finland."
                      " Earlier this week the authorities managed to isolate the first "
                      " few zombies to Ratina's football stadium but today zombies managed"
                      " to break out."
                      " Luckily, a citizen of Tampere found out to be immune"
                      " to the virus by getting no symptoms at all after being bitten "
                      " by a zombie on a business trip.";
const QString lore10 = " Today the immune citizen has returned to the town by train and is"
                      " asked by the city of Tampere to destroy as many zombies as possible "
                      " before they infect more people." ;
const QString lore11 = " The mayor of the Tampere provides a jetpack for our fearless hero to"
                       " move faster over waters and other obstacles. The mayor also advises "
                       " the immune citizen to look up for weapons from city. "
                       " According to the mayor, the city council has stored weaponry somewhere "
                       " there for situations like this. \n ";
const QString start = "\nPlease press Continue to start the game. " ;

StoryWindow::StoryWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StoryWindow),
    clicksLeft(11)
{
    ui->setupUi(this);
    connect(this, &StoryWindow::toDialog,this,&StoryWindow::accept);
}

StoryWindow::~StoryWindow()
{
    delete ui;
}

void StoryWindow::scrollLoreBrowserDown()
{
    QScrollBar *sb = ui->loreBrowser->verticalScrollBar();
    sb->setValue(sb->maximum());
}

void StoryWindow::on_continueButton_clicked()
{
    if(clicksLeft==11)
    {
        ui->loreBrowser->insertPlainText(lore1);
        --clicksLeft;
        return;
    }
    else if(clicksLeft==10)
    {
        ui->loreBrowser->insertPlainText(lore2);
        --clicksLeft;
        return;
    }
    else if(clicksLeft==9)
    {
        ui->loreBrowser->insertPlainText(lore3);
        --clicksLeft;
        return;
    }
    else if(clicksLeft==8)
    {
        ui->loreBrowser->insertPlainText(lore4);
        --clicksLeft;
        return;
    }
    else if(clicksLeft==7)
    {
        ui->loreBrowser->insertPlainText(lore5);
        --clicksLeft;
        return;
    }
    else if(clicksLeft==6)
    {
        ui->loreBrowser->insertPlainText(lore6);
        --clicksLeft;
        return;
    }
    else if(clicksLeft==5)
    {
        ui->loreBrowser->insertPlainText(lore7);
        --clicksLeft;
        return;
    }
    else if(clicksLeft==4)
    {
        ui->loreBrowser->insertPlainText(lore8);
        --clicksLeft;
        return;
    }
    else if(clicksLeft==3)
    {
        ui->loreBrowser->insertPlainText(lore9);
        --clicksLeft;
        scrollLoreBrowserDown();
        return;
    }
    else if(clicksLeft==2)
    {
        ui->loreBrowser->insertPlainText(lore10);
        scrollLoreBrowserDown();
        --clicksLeft;
        return;
    }
    else if(clicksLeft==1)
    {
        ui->loreBrowser->insertPlainText(lore11);
        ui->loreBrowser->insertPlainText(start);
        QPalette pal = ui->continueButton->palette();
        pal.setColor(QPalette::Button,QColor(Qt::green));
        ui->continueButton->setAutoFillBackground(true);
        ui->continueButton->setPalette(pal);
        ui->continueButton->update();
        --clicksLeft;
        scrollLoreBrowserDown();
        return;
    }
    emit toDialog();
}


}

