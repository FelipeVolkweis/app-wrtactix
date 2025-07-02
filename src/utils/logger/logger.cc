#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QHash>
#include <QLoggingCategory>
#include <QObject>

#include "logger.hh"
#include <iostream>

QFile *Logger::debugLogFile = Q_NULLPTR;
QFile *Logger::infoLogFile = Q_NULLPTR;

void Logger::init() {
    // set logging rules
    Logger::setLogFilters();

    QString logDir = QDir(QCoreApplication::applicationDirPath()).filePath("logs");

    // Set up logging directory
    if (!QDir(logDir).exists())
        QDir().mkdir(logDir);

    // Create log file
    debugLogFile = new QFile;
    debugLogFile->setFileName(logDir + "/debug.log");
    debugLogFile->open(QIODevice::Append | QIODevice::Text);

    infoLogFile = new QFile;
    infoLogFile->setFileName(logDir + "/info.log"); // TODO: change this to another directory
    infoLogFile->open(QIODevice::Append | QIODevice::Text);

    // Redirect logs to messageOutput
    qInstallMessageHandler(Logger::messageOutput);
}

void Logger::close() {
    if (debugLogFile != Q_NULLPTR) {
        debugLogFile->close();
        delete debugLogFile;
    }
    if (infoLogFile != Q_NULLPTR) {
        infoLogFile->close();
        delete infoLogFile;
    }
    qInstallMessageHandler(0);
}

void Logger::messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg) {

    QString category_string = context.category;

    if (category_string.contains("qt.") || category_string.contains("default") || category_string.contains("kf.")) {
        return;
    }

    QString buffer;
    QString typeName;
    switch (type) {
    case QtDebugMsg:
        typeName = "DBUG";
        break;
    case QtInfoMsg:
        typeName = "INFO";
        break;
    case QtWarningMsg:
        typeName = "WARN";
        break;
    case QtCriticalMsg:
        typeName = "CRIT";
        break;
    case QtFatalMsg:
        typeName = "FATL";
        break;
    }
    buffer = "[" + typeName + "] " + QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss.zzz") + " [" +
             category_string + "]" + " - " + msg + "\n";

    // Print to screen
    std::cout << buffer.toStdString();

    if (type == QtMsgType::QtInfoMsg) {
        infoLogFile->write(buffer.toLocal8Bit());
        infoLogFile->flush();
    }
    debugLogFile->write(buffer.toLocal8Bit());
    debugLogFile->flush();
}

void Logger::setLogFilters() {
    // method to get filter rules from a config file
    QString confDir = QDir(QCoreApplication::applicationDirPath()).filePath("logs-config");

    QFile *config = Q_NULLPTR;
    QByteArray configData = NULL;
    QString configContents = "";

    if (!QDir(confDir).exists()) {
        QDir().mkdir(confDir);
    }

    bool loggerConfigExists = QFile::exists(confDir + "/logger.conf");

    config = new QFile;
    config->setFileName(confDir + "/logger.conf");
    config->open(QIODevice::ReadWrite);

    if (!loggerConfigExists) { // Creates default logging configs
        config->write(QString("*.debug=true\n"
                              "*.info=true\n"
                              "*.warning=true\n")
                          .toLocal8Bit());
        config->flush();
    }

    configData = config->readAll();
    configContents = QString(configData);

    QLoggingCategory::setFilterRules(configContents);
}
