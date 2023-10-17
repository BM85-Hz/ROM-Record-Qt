#include "calendartab.h"

CalendarTab::CalendarTab(QWidget *parent)
    : QWidget(parent)
{
    calendar = new QCalendarWidget;
    QTextBrowser* sessionHistory = new QTextBrowser;
    sessionHistory->setPlaceholderText("No records for this date");
    notesEditor = new QTextEdit;
    notesEditor->setPlaceholderText("Notes section");
    noteSaver = new LogMaker;

    // Connect the calendar's selectionChanged signal to a slot to update notesEditor
    connect(calendar, &QCalendarWidget::selectionChanged, this, &CalendarTab::updateNotes);

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
    updateNotes();
}

void CalendarTab::updateNotes()
{
    // Get the selected date from the calendar
    QDate selectedDate = calendar->selectedDate();

    // Load notes for the selected date from storage (e.g., a file or database)
    QString notesText = loadNotes(selectedDate);

    // Set the notes text in the QTextEdit
    notesEditor->setPlainText(notesText);
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
    QString dateString = selectedDate.toString("yyyy-MM-dd");

    // Get the notes from the QTextEdit
    QString notes = notesEditor->toPlainText();

    QFile file("notes.json");

    // Save the notes to storage
    noteSaver->fileSaver(dateString, notes, file);
}
