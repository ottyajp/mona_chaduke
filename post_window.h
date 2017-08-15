#ifndef POST_WINDOW_H
#define POST_WINDOW_H

#include <QDialog>

namespace Ui {
class post_window;
}

class post_window : public QDialog
{
    Q_OBJECT

public:
    explicit post_window(QWidget *parent = 0);
    ~post_window();

private slots:
    void on_cancel_clicked();

private:
    Ui::post_window *ui;
};

#endif // POST_WINDOW_H
