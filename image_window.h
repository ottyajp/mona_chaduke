#ifndef IMAGE_WINDOW_H
#define IMAGE_WINDOW_H

#include <QDialog>

namespace Ui {
class image_window;
}

class image_window : public QDialog
{
    Q_OBJECT

public:
    explicit image_window(QWidget *parent = 0);
    ~image_window();

private:
    Ui::image_window *ui;
};

#endif // IMAGE_WINDOW_H
