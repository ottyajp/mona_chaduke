#ifndef FUNC_VAR_H
#define FUNC_VAR_H

#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QRegularExpression>

#include "network_access.h"

QString from_unix_time(int unix_time);

QString read_file(QString file_name);

QString watanabe2mona(qlonglong watanabe);

bool qvariant2bool(QVariant var);

#endif // FUNC_VAR_H
