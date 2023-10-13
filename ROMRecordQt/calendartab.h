#ifndef CALENDARTAB_H
#define CALENDARTAB_H

#include <QObject>
#include <QWidget>
#include <QCalendarWidget>
#include <QVBoxLayout>

class CalendarTab : public QWidget
{
    Q_OBJECT
public:
    CalendarTab(QWidget *parent = nullptr);
    ~CalendarTab(){}

private:
    QCalendarWidget* calendar;
};

#endif // CALENDARTAB_H
