#ifndef POST_MESSAGE_H
#define POST_MESSAGE_H

#include <QDialog>

namespace Ui {
class post_message;
}

class post_message : public QDialog
{
    Q_OBJECT

public:
    explicit post_message(QWidget *parent = 0);
    ~post_message();

private slots:

    void on_buttonBox_accepted();

private:
    Ui::post_message *ui;
};

#endif // POST_MESSAGE_H
