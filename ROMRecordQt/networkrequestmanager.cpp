#include "networkrequestmanager.h"

NetworkRequestManager::NetworkRequestManager(QObject* parent) : QObject(parent)
{
    connect(&manager, &QNetworkAccessManager::finished, this, &NetworkRequestManager::handleSearchReply);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/plain");
    request.setRawHeader("x-api-key", "DwqGnrnS3CbBHegC6TzA4sHNlKGnq4w79eD8vW43");

}

void NetworkRequestManager::sendSearchRequest(const QString& search)
{
    gameIds.clear();
    const QUrl url("https://lnattp9ct5.execute-api.us-west-2.amazonaws.com/production/v4/games");
    request.setUrl(url);

    // Puts search query into full POST request body
    QString fullInput = QString("search \"%1\"; fields name; limit 500;").arg(search);
    qDebug() << fullInput;

    QByteArray postData{fullInput.toUtf8()};

    QNetworkReply* reply = manager.post(request, postData);

    connect(reply, &QNetworkReply::finished, this, &NetworkRequestManager::handleSearchReply);
}

void NetworkRequestManager::sendGameDetailsRequest(const QString& requestString)
{
    qDebug() << requestString;

    // Construct and send the game details request
    const QUrl url("https://lnattp9ct5.execute-api.us-west-2.amazonaws.com/production/v4/games");
    request.setUrl(url);

    QByteArray postData{requestString.toUtf8()};

    QNetworkReply* reply = manager.post(request, postData);

    // Connect a slot to handle the reply
    connect(reply, &QNetworkReply::finished, this, &NetworkRequestManager::handleDetailsReply);
}

void NetworkRequestManager::handlePlatforms(QJsonArray platform_IDs){
    const QUrl url("https://lnattp9ct5.execute-api.us-west-2.amazonaws.com/production/v4/platforms");
    request.setUrl(url);

    QString requestString;
    for (auto platformID : platform_IDs){
        requestString = QString("where id = %1; fields name;").arg(platformID.toInt());
        QByteArray postData{requestString.toUtf8()};
        QNetworkReply* reply = manager.post(request, postData);
        connect(reply, &QNetworkReply::finished, this, &NetworkRequestManager::handlePlatformReply);
    }
}

void NetworkRequestManager::handleSearchReply()
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

void NetworkRequestManager::handleDetailsReply()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (reply)
    {
        if (reply->error() == QNetworkReply::NoError)
        {
            emit gameDetailsResult(reply->readAll());
        }
        else
        {
            emit gameDetailsError(reply->errorString());
        }

        reply->deleteLater();
    }
}

void NetworkRequestManager::handlePlatformReply()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    emit platformResult(reply->readAll());
}
