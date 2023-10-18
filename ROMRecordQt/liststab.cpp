#include "liststab.h"

ListsTab::ListsTab(QWidget *parent)
    : QWidget(parent)
{
    recentlyPlayed = new QTextBrowser;
    totalAmounts = new QTextBrowser;

    QFileSystemWatcher* fileWatcher = new QFileSystemWatcher;
    fileWatcher->addPath("./logs/recentslog.json");
    fileWatcher->addPath("./logs/highestlog.json");

    QVBoxLayout* mainLayout = new QVBoxLayout;
    recentLabel = new QLabel("Recently Played");
    mainLayout->addWidget(recentLabel);
    mainLayout->addWidget(recentlyPlayed);
    totalLabel = new QLabel("Total Times");
    mainLayout->addWidget(totalLabel);
    mainLayout->addWidget(totalAmounts);
    setLayout(mainLayout);
}

ListsTab::~ListsTab(){
    delete recentlyPlayed;
    delete recentLabel;
    delete totalAmounts;
    delete totalLabel;
}

QString ListsTab::loadRecents(){
    return "";
}

QString ListsTab::loadTotals(){
    return "";
}
