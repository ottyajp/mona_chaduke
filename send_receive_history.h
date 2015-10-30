#ifndef SEND_RECEIVE_HISTORY_H
#define SEND_RECEIVE_HISTORY_H

#include <QDialog>
#include <QTreeWidgetItem>

namespace Ui {
class send_receive_history;
}

class send_receive_history : public QDialog
{
    Q_OBJECT

public:
    explicit send_receive_history(QWidget *parent = 0);
    ~send_receive_history();

signals:
    void open_profile_window_from_tx();

private slots:
    void on_close_clicked();

    void on_reload_clicked();

    void on_view_itemDoubleClicked(QTreeWidgetItem *item);

private:
    Ui::send_receive_history *ui;
};

#endif // SEND_RECEIVE_HISTORY_H
