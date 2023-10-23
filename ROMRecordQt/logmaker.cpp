#include "logmaker.h"

LogMaker::LogMaker()
{

}

QString LogMaker::timestampMaker(qint64& time)
{
    int h = time / 1000 / 60 / 60;
    int m = (time / 1000 / 60) % 60;
    int s = (time / 1000) % 60;
    QString timeStamp = QString("%1:%2:%3").arg(h, 2, 10, QLatin1Char('0')).
                   arg(m, 2, 10, QLatin1Char('0')).arg(s, 2, 10, QLatin1Char('0'));

    return timeStamp;
}

QString LogMaker::addPrevious(QString& prevTimeString, qint64& clockTotal)
{
    QTime prevTime = QTime::fromString(prevTimeString, "hh:mm:ss");
    qDebug() << prevTime;

    // QTime is invalid above 23 hours(?), potentially more, need guaranteed conversion.
    qint64 prevSeconds = prevTime.hour() * 3600 + prevTime.minute() * 60 + prevTime.second();

    if (prevSeconds >= 86400){
        qint64 additionalDays = prevSeconds / 86400;
        prevSeconds %= 86400;
        prevSeconds += additionalDays * 86400;
    }

    qint64 newTotal = clockTotal + (prevSeconds * 1000);

    // Convert time into string
    QString diff = timestampMaker(newTotal);
    return diff;
}

void LogMaker::saveToLogs(QString& game, qint64& clockTotal, QDate& startDate)
{
    QFile file("./logs/calendarlog.json");

    if (file.open(QIODevice::ReadWrite  | QIODevice::Text))
    {
        // Parses json as a UTF-8 encoded JSON
        QJsonDocument jsonDocument = QJsonDocument::fromJson(file.readAll());
        // Makes JSON data struct
        QJsonObject jsonObject = jsonDocument.object();
        // Key values are dates
        QString dateString = startDate.toString("yyyy-MM-dd");

        if (!jsonObject.contains(dateString)) {
            // Create a new date key with the game and time played if not already present
            QString diff = timestampMaker(clockTotal);
            QJsonObject dateObject;
            dateObject[game] = diff;
            jsonObject[dateString] = dateObject;
        } else {
            // If the current date key exists
            QJsonObject dateObject = jsonObject[dateString].toObject();

            if (!dateObject.contains(game)) {
                // Create a new game key with the time played if not present
                QString diff = timestampMaker(clockTotal);
                dateObject[game] = diff;
                jsonObject[dateString] = dateObject;
            } else {
                // If the game key already exists
                QString prevTimeString = dateObject[game].toString();

                QString diff = addPrevious(prevTimeString, clockTotal);

                // Update the existing game key with the new time played
                dateObject[game] = diff;
                jsonObject[dateString] = dateObject;
            }
        }

        file.resize(0); // Clear the file content
        // Write the updated JSON back to the file
        file.write(QJsonDocument(jsonObject).toJson());
        file.close();
    }

    file.setFileName("./logs/highestlog.json");
    if (file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        // Parses json as a UTF-8 encoded JSON
        QJsonDocument jsonDocument = QJsonDocument::fromJson(file.readAll());
        // Makes JSON data struct
        QJsonObject jsonObject = jsonDocument.object();

        if (!jsonObject.contains(game)) {
            // Create a new date key with the game and time played if not already present
            QString diff = timestampMaker(clockTotal);
            jsonObject[game] = diff;
        } else {
            // If the game key already exists
            QString prevTimeString = jsonObject[game].toString();

            QString diff = addPrevious(prevTimeString, clockTotal);

            // Update the existing game key with the new time played
            jsonObject[game] = diff;
        }

        file.resize(0); // Clear the file content
        // Write the updated JSON back to the file
        file.write(QJsonDocument(jsonObject).toJson());
        file.close();
    }
}
