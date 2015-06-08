#include "send_mona_to_res_window.h"
#include "ui_send_mona_to_res_window.h"
#include "mainwindow.h"

send_mona_to_res_window::send_mona_to_res_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::send_mona_to_res_window)
{
    ui->setupUi(this);
    ui->send_to->setText(tr("How much mona send to >>") + send_to + tr(" ?"));
}

send_mona_to_res_window::~send_mona_to_res_window()
{
    delete ui;
}
