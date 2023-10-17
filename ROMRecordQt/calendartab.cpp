#include "calendartab.h"

CalendarTab::CalendarTab(QWidget *parent)
    : QWidget(parent)
{
    calendar = new QCalendarWidget;
    sessionHistory = new QTextBrowser;
    sessionHistory->setPlaceholderText("No records for this date");
    notesEditor = new QTextEdit;
    notesEditor->setPlaceholderText("Notes section");

    // Connect the calendar's selectionChanged signal to a slot to update notesEditor
    connect(calendar, &QCalendarWidget::selectionChanged, this, &CalendarTab::updateFields);

    // Connect the notesEditor's textChanged signal to a slot to save notes
    connect(notesEditor, &QTextEdit::textChanged, this, &CalendarTab::passNotes);

    // Create CalendarTab layout
    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(calendar);
    QVBoxLayout* layoutWithin = new QVBoxLayout;
    layoutWithin->addWidget(sessionHistory);
    layoutWithin->addWidget(notesEditor);
    mainLayout->addLayout(layoutWithin);
    setLayout(mainLayout);

    // Load and display notes for the current date
    updateFields();
}

void CalendarTab::updateFields()
{
    // Get the selected date from the calendar
    QDate selectedDate = calendar->selectedDate();

    // Load notes and play sessions for the selected date from storage (e.g., a file or database)
    QString notesText = loadNotes(selectedDate);
    QString sessionText = loadSessions(selectedDate);

    // Set the notes and play session text in the QTextEdit and QTextBrowser
    notesEditor->setPlainText(notesText);
    sessionHistory->setPlainText(sessionText);
}

QString CalendarTab::loadNotes(const QDate& date)
{
    QString notes;
    // Read notes from a file or database based on the date
    QFile file("notes.json");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // Parses json as a UTF-8 encoded JSON
        QJsonDocument jsonDocument = QJsonDocument::fromJson(file.readAll());
        // Makes JSON data struct
        QJsonObject jsonObject = jsonDocument.object();
        // Key values are dates
        QString dateString = date.toString("yyyy-MM-dd");

        // Reads and returns notes if they exist for the given date
        if (jsonObject.contains(dateString))
        {
            notes = jsonObject[dateString].toString();
        }
        file.close();
    }
    return notes;
}

void CalendarTab::passNotes() // Helper function for saving notes
{
    // Get the selected date from the calendar
    QDate selectedDate = calendar->selectedDate();

    // Get the notes from the QTextEdit
    QString notes = notesEditor->toPlainText();

    // Save the notes to storage
    saveNotes(selectedDate, notes);
}

void CalendarTab::saveNotes(const QDate& date, const QString& notes)
{
    QFile file("notes.json");
    QString dateString = date.toString("yyyy-MM-dd");

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
        jsonObject[dateString] = notes;

        jsonDocument.setObject(jsonObject);

        // Write the updated JSON data back to the file
        file.seek(0); // Move cursor to top of file
        file.write(jsonDocument.toJson()); // Write in updated contents
        file.resize(file.pos()); // Resize file to the cursor position
        file.close();
    }
}

QString CalendarTab::loadSessions(const QDate& date)
{
    QString notes;
    // Read notes from a file or database based on the date
    QFile file("calendarlog.json");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // Parses json as a UTF-8 encoded JSON
        QJsonDocument jsonDocument = QJsonDocument::fromJson(file.readAll());
        // Makes JSON data struct
        QJsonObject jsonObject = jsonDocument.object();
        // Key values are dates
        QString dateString = date.toString("yyyy-MM-dd");

        // Reads and returns notes if they exist for the given date
        if (jsonObject.contains(dateString))
        {
            notes = jsonObject[dateString].toString();
            qDebug() << notes;
        }
        file.close();
    }
    return notes;
}
