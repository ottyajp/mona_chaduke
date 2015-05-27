#ifndef USER_PROFILE_WINDOW_H
#define USER_PROFILE_WINDOW_H

#include <QDialog>

namespace Ui {
class User_Profile_window;
}

class User_Profile_window : public QDialog
{
    Q_OBJECT

public:
    explicit User_Profile_window(QWidget *parent = 0);
    ~User_Profile_window();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::User_Profile_window *ui;
};

#endif // USER_PROFILE_WINDOW_H
