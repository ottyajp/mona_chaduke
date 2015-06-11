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

signals:
    void post_success();

private slots:

    void on_buttonBox_accepted();

private:
    Ui::post_message *ui;
    void call_post_message_api();
};

#endif // POST_MESSAGE_H
