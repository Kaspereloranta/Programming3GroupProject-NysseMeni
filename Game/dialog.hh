#ifndef DIALOG_HH
#define DIALOG_HH
#include <QDialog>
#include "storywindow.hh"
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>

namespace Ui {
class Dialog;
}

namespace StudentSide
{

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    void readData(std::map<std::string,int>* dataStruct, std::string filename);
    void showData(std::map<std::string, int> *dataStruct, int diffLevel);

private slots:
    void on_startButton_clicked();

    void on_lineEdit_textEdited(const QString &arg1);

    void on_spinBox_valueChanged(int arg1);

signals:
    void start(QImage& img);
    void player_name_and_difficulty(QString name, int difficulty_level);

private:
    Ui::Dialog *ui;
    QString player_name_;
    int difficulty_level_;
};
}

#endif // DIALOG_HH
