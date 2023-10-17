#ifndef LOGMAKER_H
#define LOGMAKER_H

#include <QObject>
#include <QFile>
#include <QTime>
#include <QDate>
#include <QJsonObject>
#include <QJsonDocument>

class LogMaker : public QObject
{
    Q_OBJECT
public:
    LogMaker();

    void saveToLogs(QString& game, qint64& clockTotal);
    void fileSaver(const QString&, const QString&, QFile&);
};

#endif // LOGMAKER_H
