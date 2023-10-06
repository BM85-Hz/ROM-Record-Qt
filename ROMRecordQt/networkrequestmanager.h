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
    explicit NetworkRequestManager(QObject* parent = nullptr);
    QList<int> gameIds;

public slots:
    void sendSearchRequest(const QString& search);
    void sendGameDetailsRequest(const QString& requestString);

private slots:
    void handleNetworkReply();

signals:
    void searchResult(const QByteArray& result);
    void searchError(const QString& error);
    void gameDetailsResult(const QByteArray& result);
    void gameDetailsError(const QString& error);

private:
    QNetworkAccessManager manager;
};

#endif // NETWORKREQUESTMANAGER_H
