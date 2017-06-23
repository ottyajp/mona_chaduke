#ifndef FUNC_VAR_H
#define FUNC_VAR_H

#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QDebug>

QString from_unix_time(int unix_time);

QString read_file(QString file_name);

#endif // FUNC_VAR_H
