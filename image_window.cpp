#include "image_window.h"
#include "ui_image_window.h"
#include "mainwindow.h"
#include <QUrl>

image_Window::image_Window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::image_Window)
{
    ui->setupUi(this);
    QUrl url;
    url.setUrl(send_to);
    ui->image_view->setUrl(url);
}

image_Window::~image_Window()
{
    delete ui;
}

void image_Window::on_pushButton_clicked()
{
    close();
}
