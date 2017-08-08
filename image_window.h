#ifndef IMAGE_WINDOW_H
#define IMAGE_WINDOW_H

#include <QDialog>
#include <QWebEngineView>

namespace Ui {
class image_window;
}

class image_window : public QDialog
{
    Q_OBJECT

public:
    explicit image_window(QWidget *parent = 0);
    ~image_window();
    void setUrl(QString url);

private slots:
    void on_close_button_clicked();

private:
    Ui::image_window *ui;
};

#endif // IMAGE_WINDOW_H
