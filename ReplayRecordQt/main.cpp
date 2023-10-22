#include "mainwindow.h"

#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCoreApplication>
#include <QFile>
#include <QtCore>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

// Remember to include logs folder with build

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // Creates log directory if it doesn't already exist
    QString directoryPath = "./logs";
    QDir directory;
    if (!directory.exists(directoryPath)) {
        directory.mkpath(directoryPath);
    }

    QVarLengthArray<QString> fileList;
    fileList.append("./logs/calendarlog.json");
    fileList.append("./logs/recentslog.json");
    fileList.append("./logs/highestlog.json");

    // Generates log files on first use or after deletion
    for(auto& fileName : fileList){
        if (!QFile::exists(fileName)){
            QFile file(fileName);

            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                file.close();
            } else {
                qWarning("Could not create the file.");
            }
        }
    }

    MainWindow window;
    window.show();

    return app.exec();
}
