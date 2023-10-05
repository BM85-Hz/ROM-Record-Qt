#include "networkrequestmanager.h"

NetworkRequestManager::NetworkRequestManager(QObject* parent) : QObject(parent)
{
    connect(&manager, &QNetworkAccessManager::finished, this, &NetworkRequestManager::handleNetworkReply);
}

void NetworkRequestManager::sendSearchRequest(const QString& search)
{
    const QUrl url("https://lnattp9ct5.execute-api.us-west-2.amazonaws.com/production/v4/games");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/plain");
    request.setRawHeader("x-api-key", "DwqGnrnS3CbBHegC6TzA4sHNlKGnq4w79eD8vW43");

    QString fullInput = QString("search \"%1\"; fields name;").arg(search);
    qDebug() << fullInput;

    QByteArray postData(fullInput.toUtf8());

    QNetworkReply* reply = manager.post(request, postData);

    connect(reply, &QNetworkReply::finished, this, &NetworkRequestManager::handleNetworkReply);
}

void NetworkRequestManager::handleNetworkReply()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (reply)
    {
        if (reply->error() == QNetworkReply::NoError)
        {
            emit searchResult(reply->readAll());
        }
        else
        {
            emit searchError(reply->errorString());
        }

        reply->deleteLater();
    }
}
