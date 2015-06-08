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

private:
    Ui::send_mona_to_res_window *ui;
};

#endif // SEND_MONA_TO_RES_WINDOW_H
