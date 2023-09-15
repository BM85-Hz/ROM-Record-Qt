#include "netigdb.h"

void NetIGDB::requester(){
    //instance of QNetworkAccessManager
    manager = new QNetworkAccessManager(this);

    //set URL and headers for the POST request
    QNetworkRequest request;
    request.setUrl(QUrl("https://lnattp9ct5.execute-api.us-west-2.amazonaws.com/production/v4/games"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "x-api-key/DwqGnrnS3CbBHegC6TzA4sHNlKGnq4w79eD8vW43");

    //data i'm sending in the POST request
    QJsonObject json;
    json["body"] = "search \"mario world\"; fields name;";

    QJsonDocument jsonData(json);
    QByteArray postData = jsonData.toJson();

    //makes POST request
    reply = manager->post(request, postData);

    //connects signals and slots for error-handling
    connect(reply, &QNetworkReply::finished, this, &NetIGDB::requestFinished);
    connect(reply, &QNetworkReply::errorOccurred, this, &NetIGDB::requestError);

    manager->deleteLater();
}

void NetIGDB::requestFinished(){
    if(reply->error() == QNetworkReply::NoError){
        //process successful response in here
        QByteArray responseData = reply->readAll();
        qDebug() << "Response: " << responseData;
    }
    else{
        //handle error
        qDebug() << "Error: " << reply->errorString();
    }
    reply->deleteLater(); //clean reply object
}

void NetIGDB::requestError(QNetworkReply::NetworkError code){
    qDebug() << "Request error: " << code;
}
