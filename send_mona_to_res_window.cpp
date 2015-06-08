#include "send_mona_to_res_window.h"
#include "ui_send_mona_to_res_window.h"
#include "mainwindow.h"

send_mona_to_res_window::send_mona_to_res_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::send_mona_to_res_window)
{
    ui->setupUi(this);
    ui->send_to->setText(tr("How much mona send to >>") + send_to + tr(" ?"));
    ui->send_mona_preset1->setText("(&1)" + send_mona_amount_1);
    ui->send_mona_preset2->setText("(&2)" + send_mona_amount_2);
    ui->send_mona_preset3->setText("(&3)" + send_mona_amount_3);
    ui->send_mona_preset4->setText("(&4)" + send_mona_amount_4);
}

send_mona_to_res_window::~send_mona_to_res_window()
{
    delete ui;
}
