#ifndef GET_BALANCE_H
#define GET_BALANCE_H

#include <QDialog>

namespace Ui {
class get_balance;
}

class get_balance : public QDialog
{
    Q_OBJECT

public:
    explicit get_balance(QWidget *parent = 0);
    ~get_balance();

private:
    Ui::get_balance *ui;
};

#endif // GET_BALANCE_H
