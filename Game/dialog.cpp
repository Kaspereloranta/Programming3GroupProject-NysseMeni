#include "dialog.hh"
#include "ui_dialog.h"
#include <iostream>
#include <fstream>
#include <QDebug>

namespace StudentSide
{
const std::string DATA1 = "TOP-10-Data-Level-1.txt";
const std::string DATA2 = "TOP-10-Data-Level-2.txt";
const std::string DATA3 = "TOP-10-Data-Level-3.txt";

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    player_name_("Player"),
    difficulty_level_(1)
{
    ui->setupUi(this);
    connect(ui->startButton, &QPushButton::clicked,this,&Dialog::accept);
    StoryWindow story;
    story.exec();
    ui->startButton->setEnabled(false);

    // to read names of the players
    // and their points for top10-lists
    std::map<std::string,int> easyData;
    std::map<std::string,int> mediumData;
    std::map<std::string,int> hardData;
    readData(&easyData,DATA1);
    readData(&mediumData,DATA2);
    readData(&hardData,DATA3);

}

Dialog::~Dialog()
{    
    delete ui;
}

void Dialog::readData(std::map<std::string, int> *dataStruct, std::string filename)
{
    if(filename=="TOP-10-Data-Level-1.txt")
    {
        std::ifstream data1;
        data1.open("TOP-10-Data-Level-1.txt");
        if(!data1.fail())
        {
            std::string item;
            int itemnumber = 1;
            std::string name;
            while(!data1.eof())
            {
                if (itemnumber % 2 == 0)
                {
                    data1 >> item;
                    int points = std::stoi(item);
                    // following if-else struct
                    // is to ensure that only the highest
                    // score of each player is saved
                    if(dataStruct->find(name)==dataStruct->end())
                    {
                        dataStruct->insert({name,points});
                    }
                    else
                    {
                        if(points > dataStruct->at(name))
                        {
                            dataStruct->at(name) = points;
                        }
                    }
                }
                else
                {
                    data1 >> item;
                    name = item;
                }
                ++itemnumber;
            }
            showData(dataStruct,1);
        }
        data1.close();
        return;
    }
    else if(filename=="TOP-10-Data-Level-2.txt")
    {
        std::ifstream data2;
        data2.open("TOP-10-Data-Level-2.txt");
        if(!data2.fail())
        {
            std::string item;
            int itemnumber = 1;
            std::string name;
            while(!data2.eof())
            {
                if (itemnumber % 2 == 0)
                {
                    data2 >> item;
                    int points = std::stoi(item);
                    // following if-else struct
                    // is to ensure that only the highest
                    // score of each player is saved
                    if(dataStruct->find(name)==dataStruct->end())
                    {
                        dataStruct->insert({name,points});
                    }
                    else
                    {
                        if(points > dataStruct->at(name))
                        {
                            dataStruct->at(name) = points;
                        }
                    }
                }
                else
                {
                    data2 >> item;
                    name = item;
                }
                ++itemnumber;
            }
            showData(dataStruct,2);
        }
        data2.close();
        return;
    }
    else if(filename=="TOP-10-Data-Level-3.txt")
    {
        std::ifstream data3;
        data3.open("TOP-10-Data-Level-3.txt");
        if(!data3.fail())
        {
            std::string item;
            int itemnumber = 1;
            std::string name;
            while(!data3.eof())
            {
                if (itemnumber % 2 == 0)
                {
                    data3 >> item;
                    int points = std::stoi(item);
                    // following if-else struct
                    // is to ensure that only the highest
                    // score of each player is saved
                    if(dataStruct->find(name)==dataStruct->end())
                    {
                        dataStruct->insert({name,points});
                    }
                    else
                    {
                        if(points > dataStruct->at(name))
                        {
                            dataStruct->at(name) = points;
                        }
                    }
                }
                else
                {
                    data3 >> item;
                    name = item;
                }
                ++itemnumber;
            }
            showData(dataStruct,3);
        }
        data3.close();
        return;
    }
}

void Dialog::showData(std::map<std::string, int> *dataStruct, int diffLevel)
{
    std::vector<std::pair<int,std::string>> sortMapByValue;

    if(diffLevel==1)
    {
        for(auto scoreInClutter : *dataStruct)
        {
            sortMapByValue.push_back(std::make_pair(scoreInClutter.
                                     second,scoreInClutter.first));
        }
        sort(sortMapByValue.begin(),sortMapByValue.end());
        // to set highest score to be the first element
        // of sortMapByValue
        reverse(sortMapByValue.begin(),sortMapByValue.end());
        int howManyAddedTop10 = 0;
        for (auto scoreInOrder : sortMapByValue)
        {
            if(howManyAddedTop10<10)
            {
                std::string points = std::to_string(scoreInOrder.first);
                ui->easy->insertPlainText(QString::fromStdString(scoreInOrder.second));
                ui->easy->insertPlainText("   " + QString::fromStdString(points) + "\n");
                ++howManyAddedTop10;
            }
        }
    }
    else if(diffLevel==2)
    {
        for(auto scoreInClutter : *dataStruct)
        {
            sortMapByValue.push_back(std::make_pair(scoreInClutter.
                                     second,scoreInClutter.first));
        }
        sort(sortMapByValue.begin(),sortMapByValue.end());
        // to set highest score to be the first element
        // of sortMapByValue
        reverse(sortMapByValue.begin(),sortMapByValue.end());
        int howManyAddedTop10 = 0;
        for (auto scoreInOrder : sortMapByValue)
        {
            if(howManyAddedTop10<10)
            {
                std::string points = std::to_string(scoreInOrder.first);
                ui->medium->insertPlainText(QString::fromStdString(scoreInOrder.second));
                ui->medium->insertPlainText("   " + QString::fromStdString(points) + "\n");
                ++howManyAddedTop10;
            }
        }
    }
    else if(diffLevel==3)
    {
        for(auto scoreInClutter : *dataStruct)
        {
            sortMapByValue.push_back(std::make_pair(scoreInClutter.
                                     second,scoreInClutter.first));
        }
        sort(sortMapByValue.begin(),sortMapByValue.end());
        // to set highest score to be the first element
        // of sortMapByValue
        reverse(sortMapByValue.begin(),sortMapByValue.end());
        int howManyAddedTop10 = 0;
        for (auto scoreInOrder : sortMapByValue)
        {
            if(howManyAddedTop10<10)
            {
                std::string points = std::to_string(scoreInOrder.first);
                ui->hard->insertPlainText(QString::fromStdString(scoreInOrder.second));
                ui->hard->insertPlainText("   " + QString::fromStdString(points) + "\n");
                ++howManyAddedTop10;
            }
        }
    }
}

void StudentSide::Dialog::on_startButton_clicked()
{
   emit player_name_and_difficulty(player_name_,difficulty_level_);
}

void StudentSide::Dialog::on_lineEdit_textEdited(const QString &arg1)
{
    player_name_= arg1;
    ui->startButton->setEnabled(true);
}


void StudentSide::Dialog::on_spinBox_valueChanged(int arg1)
{
    difficulty_level_ = arg1;
}
}
