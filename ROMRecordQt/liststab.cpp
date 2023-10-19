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
    recentLabel = new QLabel("10 Most Recently Played Games");
    mainLayout->addWidget(recentLabel);
    mainLayout->addWidget(recentlyPlayed);
    totalLabel = new QLabel("Total Times (in Descending Order)");
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
    QString recentsText = loadRecents();
    recentlyPlayed->setPlainText(recentsText);
    QString totalsText = loadTotals();
    totalAmounts->setPlainText(totalsText);
}

QString ListsTab::loadRecents(){
    QString records;
    QFile file("./logs/recentslog.json");

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // Parses json as a UTF-8 encoded JSON
        QJsonDocument jsonDocument = QJsonDocument::fromJson(file.readAll());
        // Makes JSON data struct
        QJsonObject jsonObject = jsonDocument.object();

        for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it)
        {
            QString gameName = it.value().toString();

            // Concatenate the results into the QString
            records += gameName + "\n";
        }
        file.close();
    }
    return records;
}

QString ListsTab::loadTotals(){
    QString records;
    QFile file("./logs/highestlog.json");

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // Parses json as a UTF-8 encoded JSON
        QJsonDocument jsonDocument{QJsonDocument::fromJson(file.readAll())};
        // Makes JSON data struct
        QJsonObject jsonObject{jsonDocument.object()};

        QVector<QPair<QString, QString>> timestampVector;

        // Add JSON data to vector
        for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it) {
            QString gameName{it.key()};
            QString timestamp{it.value().toString()};
            timestampVector.append(qMakePair(gameName, timestamp));
        }
        file.close();

        // Custom sorting function that converts timestamps to seconds and compares
        // Needed because otherwise the strings will not sort properly
        // (i.e. they will attempt to adhere to hh:mm:ss format even though
        // the hours place is not restricted to two digits)
        auto descendingOrder{
            [](const QPair<QString, QString>& a, const QPair<QString, QString>& b) {
                // Take two timestamps using the lambda declaration above
                QString timestampA{a.second};
                QString timestampB{b.second};

                // An integer we can compare reliably
                int secondsA{};
                int secondsB{};

                // Parse and convert timestamps to seconds
                static QRegularExpression regex("(\\d+):(\\d+):(\\d+)");
                QRegularExpressionMatch matchA{regex.match(timestampA)};
                QRegularExpressionMatch matchB{regex.match(timestampB)};

                // Use the regex captures to convert to seconds
                if (matchA.hasMatch()) {
                    secondsA = matchA.captured(1).toInt() * 3600 +
                               matchA.captured(2).toInt() * 60 + matchA.captured(3).toInt();
                }

                if (matchB.hasMatch()) {
                    secondsB = matchB.captured(1).toInt() * 3600 +
                               matchB.captured(2).toInt() * 60 + matchB.captured(3).toInt();
                }

                return secondsA > secondsB;
            }
        };

        std::sort(timestampVector.begin(), timestampVector.end(), descendingOrder);

        // Concatenate the results into the QString
        for (const auto& pair : timestampVector) {
            QString gameName{pair.first};
            QString timestamp{pair.second};
            records += gameName + ": " + timestamp + "\n";
        }
    }
    return records;
}
