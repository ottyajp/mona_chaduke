#ifndef SEND_MONA_TO_RES_WINDOW_H
#define SEND_MONA_TO_RES_WINDOW_H

#include <QDialog>

namespace Ui {
class send_mona_to_res_window;
}

class send_mona_to_res_window : public QDialog
{
    Q_OBJECT

public:
    explicit send_mona_to_res_window(QWidget *parent = 0);
    ~send_mona_to_res_window();

signals:
    void send_success();

private slots:
    void on_send_button_clicked();

    void on_send_mona_preset1_clicked();

    void on_send_mona_preset2_clicked();

    void on_send_mona_preset3_clicked();

    void on_send_mona_preset4_clicked();

    void on_anonymous_check_stateChanged(int arg1);

private:
    Ui::send_mona_to_res_window *ui;
    void send_success_fire();
};

#endif // SEND_MONA_TO_RES_WINDOW_H
