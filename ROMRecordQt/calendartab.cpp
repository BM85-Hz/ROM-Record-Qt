#include "calendartab.h"

CalendarTab::CalendarTab(QWidget *parent)
    : QWidget(parent)
{
    // Create objects for the Calendar Tab
    calendar = new QCalendarWidget;
    calendar->setStyleSheet("color: #000000;");
    sessionLabel = new QLabel;
    sessionHistory = new QTextBrowser;
    sessionHistory->setPlaceholderText("No records for this date");
    noteLabel = new QLabel("Notes");
    notesEditor = new QTextEdit;
    notesEditor->setPlaceholderText("Write something!");

    // Connect the calendar's selectionChanged signal to a slot to update sessionHistory and notesEditor
    connect(calendar, &QCalendarWidget::selectionChanged, this, &CalendarTab::updateSessions);
    connect(calendar, &QCalendarWidget::selectionChanged, this, &CalendarTab::updateNotes);

    // Connect the notesEditor's textChanged signal to a slot to save notes
    connect(notesEditor, &QTextEdit::textChanged, this, &CalendarTab::passNotes);

    // Watch "calendarlog.json" for changes
    QFileSystemWatcher* fileWatcher = new QFileSystemWatcher;
    fileWatcher->addPath("./logs/calendarlog.json");

    // Connect the fileChanged signal to a slot to handle file updates
    connect(fileWatcher, &QFileSystemWatcher::fileChanged, this, &CalendarTab::updateSessions);

    // Create CalendarTab layout
    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(calendar);
    QVBoxLayout* layoutWithin = new QVBoxLayout;
    layoutWithin->addWidget(sessionLabel);
    layoutWithin->addWidget(sessionHistory);
    layoutWithin->addWidget(noteLabel);
    layoutWithin->addWidget(notesEditor);
    mainLayout->addLayout(layoutWithin);
    setLayout(mainLayout);

    // Load and display notes for the current date
    updateSessions();
    updateNotes();
}

CalendarTab::~CalendarTab(){
    delete calendar;
    delete sessionHistory;
    delete notesEditor;
    delete sessionLabel;
    delete noteLabel;
}

void CalendarTab::updateSessions()
{
    // Get the selected date from the calendar
    QDate selectedDate = calendar->selectedDate();

    // Load play sessions for the selected date from storage
    QString sessionText = loadSessions(selectedDate);

    // Set the play session text in the QTextBrowser
    sessionHistory->setPlainText(sessionText);

    QString labelText = QString("Games Played on %1").
                        arg(selectedDate.toString("dddd, MMMM d, yyyy"));
    sessionLabel->setText(labelText);
}

void CalendarTab::updateNotes()
{
    // Similar to previous function; originally had one function for
    // both notes and play sessions, but reloading notes every time
    // the play session games updated is potentially inefficient
    QDate selectedDate = calendar->selectedDate();

    QString notesText = loadNotes(selectedDate);

    notesEditor->setPlainText(notesText);
}

QString CalendarTab::loadNotes(const QDate& date)
{
    QString notes;
    // Read notes from a file or database based on the date
    QFile file("./logs/notes.json");
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
    QFile file("./logs/notes.json");
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
    QString records;
    QFile file("./logs/calendarlog.json");

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
            // Get the nested JSON object for the selected date
            QJsonObject dateData = jsonObject[dateString].toObject();

            // Iterate through the games and timestamps using pointer arithmetic
            for (auto it = dateData.begin(); it != dateData.end(); ++it)
            {
                QString gameName = it.key();
                QString timestamp = it.value().toString();

                // Concatenate the results into the QString
                records += "-" + gameName + ": " + timestamp + "\n";
            }
        }
        file.close();
    }
    return records;
}
