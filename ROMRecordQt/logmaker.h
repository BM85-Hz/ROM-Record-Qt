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
    QString timestampMaker(qint64& time);
    void saveToLogs(QString& game, qint64& clockTotal);

private:
    void logSaver(const QString&, const QString&, QFile&);
};

#endif // LOGMAKER_H
