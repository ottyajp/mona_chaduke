#ifndef NEW_TOPIC_H
#define NEW_TOPIC_H

#include <QDialog>

namespace Ui {
class new_Topic;
}

class new_Topic : public QDialog
{
    Q_OBJECT

public:
    explicit new_Topic(QWidget *parent = 0);
    ~new_Topic();

private:
    Ui::new_Topic *ui;
};

#endif // NEW_TOPIC_H
