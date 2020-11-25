#ifndef STORYWINDOW_HH
#define STORYWINDOW_HH

#include <QDialog>


namespace Ui {
class StoryWindow;
}
namespace StudentSide
{

class StoryWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StoryWindow(QWidget *parent = nullptr);
    ~StoryWindow();
    void scrollLoreBrowserDown();

private slots:
    void on_continueButton_clicked();

signals:
    void toDialog();

private:
    Ui::StoryWindow *ui;
    int clicksLeft;
};
}
#endif // STORYWINDOW_HH
