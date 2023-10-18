#ifndef CALENDARTAB_H
#define CALENDARTAB_H

#include <QObject>
#include <QWidget>
#include <QCalendarWidget>
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QTextEdit>
#include <QFile>
#include <QFileSystemWatcher>
#include <QTextStream>
#include <QDate>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>

class CalendarTab : public QWidget
{
    Q_OBJECT
public:
    explicit CalendarTab(QWidget *parent = nullptr);
    ~CalendarTab();

private slots:
    void updateSessions();
    void updateNotes();
    QString loadNotes(const QDate& date);
    void passNotes();
    void saveNotes(const QDate& date, const QString& notes);
    QString loadSessions(const QDate& date);

private:
    QCalendarWidget* calendar;
    QTextEdit* notesEditor;
    QTextBrowser* sessionHistory;

    QLabel* sessionLabel;
    QLabel* noteLabel;
};

#endif // CALENDARTAB_H
