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
#include "logmaker.h"

class CalendarTab : public QWidget
{
    Q_OBJECT
public:
    CalendarTab(QWidget *parent = nullptr);
    ~CalendarTab(){}\

private slots:
    void updateNotes();
    QString loadNotes(const QDate& date);
    void passNotes();

private:
    QCalendarWidget* calendar;
    QTextEdit* notesEditor;
    LogMaker* noteSaver;
};

#endif // CALENDARTAB_H
