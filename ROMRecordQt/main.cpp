#include "mainwindow.h"
#include "networkrequestmanager.h"

#include <QApplication>
//#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCoreApplication>
#include <QtCore>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QDebug>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    NetworkRequestManager requestManager;

    // Connect to signals
    QObject::connect(&requestManager, &NetworkRequestManager::searchResult, [&](const QByteArray& result) {
        qDebug() << "Request succeeded. Response:";
        qDebug() << result;
    });

    QObject::connect(&requestManager, &NetworkRequestManager::searchError, [&](const QString& error) {
        qDebug() << "Request failed. Error:" << error;
    });

    // Send a search request
    requestManager.sendSearchRequest("mario world");
    requestManager.sendSearchRequest("zelda");
    requestManager.sendSearchRequest("metroid");

    MainWindow window;
    window.show();

    return app.exec();
}
