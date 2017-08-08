#include "image_window.h"
#include "ui_image_window.h"

image_window::image_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::image_window)
{
    ui->setupUi(this);
}

image_window::~image_window()
{
    delete ui;
}
