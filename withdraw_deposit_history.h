#ifndef WITHDRAW_DEPOSIT_HISTORY_H
#define WITHDRAW_DEPOSIT_HISTORY_H

#include <QDialog>

namespace Ui {
class withdraw_deposit_history;
}

class withdraw_deposit_history : public QDialog
{
    Q_OBJECT

public:
    explicit withdraw_deposit_history(QWidget *parent = 0);
    ~withdraw_deposit_history();

signals:

private slots:
    void on_close_clicked();

    void on_reload_clicked();

private:
    Ui::withdraw_deposit_history *ui;
};

#endif // WITHDRAW_DEPOSIT_HISTORY_H
