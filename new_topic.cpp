#include "new_topic.h"
#include "ui_new_topic.h"

new_Topic::new_Topic(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::new_Topic)
{
    ui->setupUi(this);
}

new_Topic::~new_Topic()
{
    delete ui;
}
