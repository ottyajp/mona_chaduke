#ifndef WITHDRAW_WINDOW_H
#define WITHDRAW_WINDOW_H

#include <QDialog>

namespace Ui {
class withdraw_window;
}

class withdraw_window : public QDialog
{
    Q_OBJECT

public:
    explicit withdraw_window(QWidget *parent = 0);
    ~withdraw_window();

private slots:
    void on_cancel_button_clicked();

    void on_withdraw_button_clicked();

private:
    Ui::withdraw_window *ui;
};

#endif // WITHDRAW_WINDOW_H
