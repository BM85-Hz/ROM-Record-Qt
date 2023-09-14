#include "netigdb.h"

void NetIGDB::requester(){
    QNetworkRequest request;
    request.setUrl(QUrl("https://lnattp9ct5.execute-api.us-west-2.amazonaws.com/production/v4/games"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "x-api-key/DwqGnrnS3CbBHegC6TzA4sHNlKGnq4w79eD8vW43");
}
