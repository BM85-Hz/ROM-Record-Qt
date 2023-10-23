#include "liststab.h"

// This entire class works similarly to the functionality of the CalendarTab,
// however it does not make use of dates and is thus simpler. Could be refactored if I
// want to be less lazy.

ListsTab::ListsTab(QWidget *parent)
    : QWidget(parent)
{
    recentlyPlayed = new QTextBrowser;
    totalAmounts = new QTextBrowser;

    QFileSystemWatcher* fileWatcher = new QFileSystemWatcher;
    fileWatcher->addPath("./logs/recentslog.json");
    fileWatcher->addPath("./logs/highestlog.json");

    connect(fileWatcher, &QFileSystemWatcher::fileChanged, this, &ListsTab::updateBrowsers);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    recentLabel = new QLabel("Recently Played");
    mainLayout->addWidget(recentLabel);
    mainLayout->addWidget(recentlyPlayed);
    totalLabel = new QLabel("Total Times");
    mainLayout->addWidget(totalLabel);
    mainLayout->addWidget(totalAmounts);
    setLayout(mainLayout);

    updateBrowsers();
}

ListsTab::~ListsTab(){
    delete recentlyPlayed;
    delete recentLabel;
    delete totalAmounts;
    delete totalLabel;
}

void ListsTab::updateBrowsers(){
    QString totalsText = loadTotals();
    totalAmounts->setPlainText(totalsText);
}

QString ListsTab::loadRecents(){
    return "";
}

QString ListsTab::loadTotals(){
    QString records;
    QFile file("./logs/highestlog.json");

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // Parses json as a UTF-8 encoded JSON
        QJsonDocument jsonDocument = QJsonDocument::fromJson(file.readAll());
        // Makes JSON data struct
        QJsonObject jsonObject = jsonDocument.object();

        for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it)
        {
            QString gameName = it.key();
            QString timestamp = it.value().toString();

            // Concatenate the results into the QString
            records += gameName + ": " + timestamp + "\n";
        }
        file.close();
    }
    return records;
}
