#ifndef NETIGDB_H
#define NETIGDB_H

#include <QObject>
#include <QtCore>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

class NetIGDB : public QObject{ //inheritance required
    Q_OBJECT //this macro is needed for moc to find the class
    QNetworkAccessManager* manager;
    QNetworkReply* reply;
private:
    void requester();
    void requestFinished();
    void requestError(QNetworkReply::NetworkError code);
public:
    NetIGDB(){}
    ~NetIGDB(){}
};

#endif // NETIGDB_H
