#ifndef NETWORKREQUESTMANAGER_H
#define NETWORKREQUESTMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class NetworkRequestManager : public QObject
{
    Q_OBJECT

public:
    NetworkRequestManager(QObject* parent = nullptr);

public slots:
    void sendSearchRequest(const QString& search);

private slots:
    void handleNetworkReply();

signals:
    void searchResult(const QByteArray& result);
    void searchError(const QString& error);

private:
    QNetworkAccessManager manager;
};

#endif // NETWORKREQUESTMANAGER_H
