#include "liststab.h"

ListsTab::ListsTab(QWidget *parent)
    : QWidget(parent)
{
    recentlyPlayed = new QTextBrowser;
    totalAmounts = new QTextBrowser;

    QVBoxLayout* mainLayout = new QVBoxLayout;
    recentLabel = new QLabel("Recently Played");
    mainLayout->addWidget(recentLabel);
    mainLayout->addWidget(recentlyPlayed);
    totalLabel = new QLabel("Total Times");
    mainLayout->addWidget(totalLabel);
    mainLayout->addWidget(totalAmounts);
    setLayout(mainLayout);
}
