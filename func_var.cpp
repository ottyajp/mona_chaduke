#include "func_var.h"

QString from_unix_time(int unix_time){
    QDateTime from;
    from.setTime_t(unix_time);
    return from.toString("yyyy/MM/dd hh:mm:ss");
}
