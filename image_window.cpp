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

void image_window::setUrl(QString url){
    QWebEngineView *image = new QWebEngineView();
    ui->verticalLayout->addWidget(image);
    ui->verticalLayout->setDirection(QBoxLayout::BottomToTop);
    image->setUrl(QUrl(url));
}

void image_window::on_close_button_clicked()
{
    this->close();
}
