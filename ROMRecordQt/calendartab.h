#ifndef CALENDARTAB_H
#define CALENDARTAB_H

#include <QObject>
#include <QWidget>
#include <QCalendarWidget>
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QJsonDocument>
#include <QJsonObject>

class CalendarTab : public QWidget
{
    Q_OBJECT
public:
    CalendarTab(QWidget *parent = nullptr);
    ~CalendarTab(){}\

private slots:
    void updateFields();
    QString loadNotes(const QDate& date);
    void passNotes();
    void saveNotes(const QDate& date, const QString& notes);
    QString loadSessions(const QDate& date);

private:
    QCalendarWidget* calendar;
    QTextEdit* notesEditor;
    QTextBrowser* sessionHistory;
};

#endif // CALENDARTAB_H
