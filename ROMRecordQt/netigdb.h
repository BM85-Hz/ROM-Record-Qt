#ifndef NETIGDB_H
#define NETIGDB_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

class NetIGDB{
    Q_OBJECT
private:
    QNetworkAccessManager* manager;
    void requester();
public:
    NetIGDB(){}
    ~NetIGDB(){}
};

#endif // NETIGDB_H
