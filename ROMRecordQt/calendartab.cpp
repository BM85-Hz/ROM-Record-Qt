#include "calendartab.h"

CalendarTab::CalendarTab(QWidget *parent)
    : QWidget(parent)
{
    calendar = new QCalendarWidget;
    QTextBrowser* sessionHistory = new QTextBrowser;
    sessionHistory->setPlaceholderText("No records for this date");
    notesEditor = new QTextEdit;
    notesEditor->setPlaceholderText("Notes section");
    noteBook = new QMap<QDate, QString>;

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
    QFile file("notes.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        QString datePattern = QString("^\\d{4}-\\d{2}-\\d{2}");
        static QRegularExpression dateRegex(datePattern);

        while (!in.atEnd())
        {
            QString line = in.readLine();
            if (line.startsWith(date.toString("yyyy-MM-dd")))
            {
                notes = line.mid(11);
                while (!in.atEnd())
                {
                    QString nextLine = in.readLine();
                    if (!nextLine.contains(dateRegex)) {
                        notes += "\n" + nextLine;
                    } else {
                        break;
                    }
                }
                break;
            }
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
    QFile file("notes.txt");

    if (file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QTextStream in(&file);
        QString updatedContents;
        bool updated = false;

        while (!in.atEnd())
        {
            QString line = in.readLine();
            if (line.startsWith(date.toString("yyyy-MM-dd")))
            {
                updatedContents.append(date.toString("yyyy-MM-dd") + " " + notes + "\n");
                updated = true;
            }
            else
            {
                updatedContents.append(line + "\n");
            }
        }

        if (!updated)
        {
            updatedContents.append(date.toString("yyyy-MM-dd") + " " + notes + "\n");
        }

        file.seek(0); // Move cursor to top of file
        file.write(updatedContents.toUtf8()); // Write in updated contents
        file.resize(file.pos()); // Resize file to the cursor position
        file.close();
    }
}
