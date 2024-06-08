#ifndef PROCESS_H
#define PROCESS_H

#include <QProcess>
#include <QPlainTextEdit>

QString runProcess(QString command, QStringList arguments, QPlainTextEdit* plainTextEdit = nullptr, QString input = QString());

#endif // PROCESS_H
