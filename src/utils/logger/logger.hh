#ifndef LOGGER_H
#define LOGGER_H

#include <QDebug>
#include <QFile>
#include <QHash>

class Logger {
public:
    static void init();
    static void close();

private:
    static void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static void setLogFilters();

private:
    static QFile *debugLogFile;
    static QFile *infoLogFile;
    static QHash<QtMsgType, QString> contextNames;
};

#endif // LOGGER_H
