#include "send_mona_to_res_window.h"
#include "ui_send_mona_to_res_window.h"

send_mona_to_res_window::send_mona_to_res_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::send_mona_to_res_window)
{
    ui->setupUi(this);
}

send_mona_to_res_window::~send_mona_to_res_window()
{
    delete ui;
}
