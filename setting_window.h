#ifndef SETTING_WINDOW_H
#define SETTING_WINDOW_H

#include <QDialog>

namespace Ui {
class Setting_window;
}

class Setting_window : public QDialog
{
    Q_OBJECT

public:
    explicit Setting_window(QWidget *parent = 0);
    ~Setting_window();

private slots:
    void on_pushButton_clicked();

    void on_buttonBox_accepted();

    void on_u_address_textChanged();

    void on_pass_textChanged();

    void on_show_state_log_check_stateChanged();

private:
    Ui::Setting_window *ui;
};

#endif // SETTING_WINDOW_H
