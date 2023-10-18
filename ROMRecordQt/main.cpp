#include "mainwindow.h"

#include <QApplication>
//#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCoreApplication>
#include <QFile>
#include <QtCore>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QDebug>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QVarLengthArray<QString> fileList;
    fileList.append("calendarlog.json");
    fileList.append("recentslog.json");
    fileList.append("highestlog.json");

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
