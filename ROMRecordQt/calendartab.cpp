#include "calendartab.h"

CalendarTab::CalendarTab(QWidget *parent)
    : QWidget(parent)
{
    calendar = new QCalendarWidget;

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(calendar);
    setLayout(mainLayout);
}
