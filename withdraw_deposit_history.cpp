#include "withdraw_deposit_history.h"
#include "ui_withdraw_deposit_history.h"
#include "mainwindow.h"
#include <QDebug>

withdraw_deposit_history::withdraw_deposit_history(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::withdraw_deposit_history)
{
    ui->setupUi(this);

    ui->view->setColumnCount(3);
    ui->view->setHeaderLabels(QStringList()<<tr("Date")<<tr("type")<<tr("amount"));
    ui->view->setColumnWidth(0,150);
    ui->view->setColumnWidth(1,60);
//    ui->view->setColumnWidth(2,100);

    emit on_reload_clicked();
}

withdraw_deposit_history::~withdraw_deposit_history()
{
    delete ui;
}


void withdraw_deposit_history::on_close_clicked()
{
    delete this;
}

void withdraw_deposit_history::on_reload_clicked()
{
    //delete all item
    for (;;){
        if(ui->view->invisibleRootItem()->child(0) == 0){break;}
        delete ui->view->invisibleRootItem()->child(0);
    }
    status_bar->showMessage(tr("get deposit history."));
    //get history
    QJsonDocument depo_json = get_tx_history("deposit","10");
    QThread::sleep(1);
    QJsonDocument with_json = get_tx_history("withdraw","10");

    for(int i=0;i<10;i++){
        if(depo_json.object().value("transactions").toArray().at(i).toObject().value("created").toInt() == 0){break;}
        QString created = from_unix_time(depo_json.object().value("transactions").toArray().at(i).toObject().value("created").toInt());
        QString type = depo_json.object().value("transactions").toArray().at(i).toObject().value("item").toString();
        if(type == "deposit") type = tr("deposit");
        if(type == "withdraw") type = tr("withdraw");
        QString amount = QString::number(
                    depo_json.object().value("transactions").toArray().at(i).toObject().value("amount").toString().toDouble() / 100000000,'f',8)
                .replace(QRegularExpression("[0]*$"),"").replace(QRegularExpression("\\.$"),"");
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0,created);
        item->setText(1,type);
        item->setText(2,amount);
        ui->view->addTopLevelItem(item);
    }
    for(int i=0;i<10;i++){
        if(with_json.object().value("transactions").toArray().at(i).toObject().value("created").toInt() == 0)break;
        QString created = from_unix_time(with_json.object().value("transactions").toArray().at(i).toObject().value("created").toInt());
        QString type = with_json.object().value("transactions").toArray().at(i).toObject().value("item").toString();
        QString amount = QString::number(
                    with_json.object().value("transactions").toArray().at(i).toObject().value("amount").toString().toDouble() / 100000000,'f',8)
                .replace(QRegularExpression("[0]*$"),"").replace(QRegularExpression("\\.$"),"");
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0,created);
        item->setText(1,type);
        item->setText(2,amount);
        ui->view->addTopLevelItem(item);
    }
    ui->view->sortByColumn(1,Qt::DescendingOrder);
}
