#include "logmaker.h"

LogMaker::LogMaker()
{
    // LogMaker just updates the logs; could have a constructor, but I decided against it
}

// Helper function that converts seconds value integer into a timestamp string
QString LogMaker::timestampMaker(qint64& time)
{
    int h = time / 1000 / 60 / 60;
    int m = (time / 1000 / 60) % 60;
    int s = (time / 1000) % 60;
    QString timeStamp = QString("%1:%2:%3").arg(h, 2, 10, QLatin1Char('0')).
                   arg(m, 2, 10, QLatin1Char('0')).arg(s, 2, 10, QLatin1Char('0'));

    return timeStamp;
}

// Takes previous value (string) in our log and converts it into an integer
// so that it can be added to the time just recorded
QString LogMaker::addPrevious(QString& prevTimeString, qint64& clockTotal)
{
    static QRegularExpression regex("(\\d+):(\\d+):(\\d+)");
    QRegularExpressionMatch match = regex.match(prevTimeString);

    if (match.hasMatch())
    {
        // Extract the matched values as strings
        QString hours = match.captured(1);
        QString minutes = match.captured(2);
        QString seconds = match.captured(3);

        // Convert the extracted strings to integers
        qint64 hoursInt = hours.toInt();
        qint64 minutesInt = minutes.toInt();
        qint64 secondsInt = seconds.toInt();

        qint64 prevSeconds = hoursInt * 3600 + minutesInt * 60 + secondsInt;

        qint64 newTotal = clockTotal + (prevSeconds * 1000);

        // Convert time into string
        QString diff = timestampMaker(newTotal);
        return diff;
    } else {
        // This does nothing, just mitigates code smell for unopposed return value
        return prevTimeString;
    }
}

// Updates all log files except for the notes
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

        if (!jsonObject.contains(dateString))
        {
            // Create a new date key with the game and time played if not already present
            QString timestamp = timestampMaker(clockTotal);
            QJsonObject dateObject;
            dateObject[game] = timestamp;
            jsonObject[dateString] = dateObject;
        } else {
            // If the current date key exists
            QJsonObject dateObject = jsonObject[dateString].toObject();

            if (!dateObject.contains(game))
            {
                // Create a new game key with the time played if not present
                QString timestamp = timestampMaker(clockTotal);
                dateObject[game] = timestamp;
                jsonObject[dateString] = dateObject;
            } else {
                // If the game key already exists
                QString prevTimeString = dateObject[game].toString();

                QString timestamp = addPrevious(prevTimeString, clockTotal);

                // Update the existing game key with the new time played
                dateObject[game] = timestamp;
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

        if (!jsonObject.contains(game))
        {
            // Create a new date key with the game and time played if not already present
            QString timestamp  = timestampMaker(clockTotal);
            jsonObject[game] = timestamp;
        } else {
            // If the game key already exists
            QString prevTimeString = jsonObject[game].toString();

            QString timestamp = addPrevious(prevTimeString, clockTotal);

            // Update the existing game key with the new time played
            jsonObject[game] = timestamp;
        }

        file.resize(0); // Clear the file content
        // Write the updated JSON back to the file
        file.write(QJsonDocument(jsonObject).toJson());
        file.close();
    }

    file.setFileName("./logs/recentslog.json");
    if (file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        // Parses json as a UTF-8 encoded JSON
        QJsonDocument jsonDocument = QJsonDocument::fromJson(file.readAll());
        // Makes JSON data struct
        QJsonObject jsonObject = jsonDocument.object();

        if (!jsonObject.contains("1"))
        { // Fills out file structure for first time use
            for (qint64 i = 0; i < 10; ++i)
            {
                QString j = QString::number(i);
                jsonObject[j] = "";
            }
            jsonObject["0"] = game;
        } else {
            // Check if the new game is already present in the JSON object
            bool isDuplicate = false;
            qint64 duplicateIndex = -1;

            for (qint64 i = 0; i < 10; ++i)
            {
                QString j = QString::number(i);
                if (jsonObject[j] == game)
                {
                    isDuplicate = true;
                    duplicateIndex = i;
                    break; // Breaks loop when duplicate is found
                }
            }

            // If duplicate is not present, we start at the back of the JSON, and slide each item back
            if (!isDuplicate)
            {
                for (qint64 i = 9; i > 0; --i)
                {
                    QString currentKey = QString::number(i);
                    QString previousKey = QString::number(i - 1);
                    jsonObject[currentKey] = jsonObject[previousKey].toString();
                }
                jsonObject["0"] = game;
            } else { // Otherwise we start at where we found the duplicate and slide accordingly
                for (qint64 i = duplicateIndex; i > 0; --i)
                {
                    QString currentKey = QString::number(i);
                    QString previousKey = QString::number(i - 1);
                    jsonObject[currentKey] = jsonObject[previousKey].toString();
                }
                jsonObject["0"] = game;
            }
        }

        file.resize(0); // Clear the file content
        // Write the updated JSON back to the file
        file.write(QJsonDocument(jsonObject).toJson());
        file.close();
    }
}
