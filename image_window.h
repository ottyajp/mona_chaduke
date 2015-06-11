#ifndef IMAGE_WINDOW_H
#define IMAGE_WINDOW_H

#include <QDialog>

namespace Ui {
class image_Window;
}

class image_Window : public QDialog
{
    Q_OBJECT

public:
    explicit image_Window(QWidget *parent = 0);
    ~image_Window();

private slots:
    void on_pushButton_clicked();

private:
    Ui::image_Window *ui;
};

#endif // IMAGE_WINDOW_H
