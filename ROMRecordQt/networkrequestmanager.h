#ifndef NETWORKREQUESTMANAGER_H
#define NETWORKREQUESTMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonArray>

class NetworkRequestManager : public QObject
{
    Q_OBJECT

public:
    explicit NetworkRequestManager(QObject* parent = nullptr);
    QList<int> gameIds;
    QNetworkRequest request;

public slots:
    void sendSearchRequest(const QString& search);
    void sendGameDetailsRequest(const QString& requestString);
    void handlePlatforms(QJsonArray platform_IDs);

private slots:
    void handleSearchReply();
    void handleDetailsReply();
    void handlePlatformReply();

signals:
    void searchResult(const QByteArray& result);
    void searchError(const QString& error);
    void gameDetailsResult(const QByteArray& result);
    void gameDetailsError(const QString& error);
    void platformResult(const QByteArray& result);

private:
    QNetworkAccessManager manager;
    int platformRequests{};
    int completedPlatformRequests{};
};

#endif // NETWORKREQUESTMANAGER_H
