#ifndef NEW_TOPIC_H
#define NEW_TOPIC_H

#include <QDialog>
#include "func_var.h"

namespace Ui {
class new_Topic;
}

class new_Topic : public QDialog
{
    Q_OBJECT

public:
    explicit new_Topic(QWidget *parent = 0);
    ~new_Topic();

private slots:

    void on_cancel_clicked();

    void on_create_topic_clicked();

private:
    Ui::new_Topic *ui;
};

#endif // NEW_TOPIC_H
