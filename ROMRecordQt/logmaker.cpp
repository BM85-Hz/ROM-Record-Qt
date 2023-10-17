#include "logmaker.h"

LogMaker::LogMaker()
{

}

void LogMaker::saveToLogs(QString& game, qint64& clockTotal){

}

void LogMaker::fileSaver(const QString& key, const QString& value, QFile& file){
    if (file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QJsonDocument jsonDocument;
        \
            // Read existing JSON data from the file
            QByteArray data = file.readAll();
        if (!data.isEmpty())
        {
            // Parse existing JSON for QJsonDocument
            jsonDocument = QJsonDocument::fromJson(data);
        }
        else
        {
            // If the file is empty, create a new JSON object
            jsonDocument.setObject(QJsonObject());
        }

        // Convert JSONDoc "back up" into JSONObject data struct
        QJsonObject jsonObject = jsonDocument.object();

        // Update or add a new entry in the JSON object
        jsonObject[key] = value;

        jsonDocument.setObject(jsonObject);

        // Write the updated JSON data back to the file
        file.seek(0); // Move cursor to top of file
        file.write(jsonDocument.toJson()); // Write in updated contents
        file.resize(file.pos()); // Resize file to the cursor position
        file.close();
    }
}
