#include "searchtab.h"

SearchTab::SearchTab(QWidget *parent)
    : QWidget(parent)
{
    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setStyleSheet("background-color: #EAEBFF;");
    resultListWidget = new QListWidget(this);
    resultListWidget->setStyleSheet("background-color: #EAEBFF;");
    textBrowserWidget = new QTextBrowser(this);
    textBrowserWidget->setStyleSheet("background-color: #EAEBFF;");
    imageLabel = new QLabel(this);
    imageLabel->setText("No cover loaded");
    stopwatch = new Stopwatch;

    // Connect signals and slots
    connect(&requestManager, &NetworkRequestManager::searchResult, this, &SearchTab::handleSearchResult);
    connect(&requestManager, &NetworkRequestManager::searchError, this, &SearchTab::handleSearchError);
    connect(&requestManager, &NetworkRequestManager::gameDetailsResult, this, &SearchTab::handleDetailsResult);
    connect(&requestManager, &NetworkRequestManager::coverResult, this, &SearchTab::handleCoverResult);
    connect(&requestManager, &NetworkRequestManager::platformResult, this, &SearchTab::handlePlatformsResult);
    connect(&requestManager, &NetworkRequestManager::companyResult, this, &SearchTab::handleCompaniesResult);
    connect(&requestManager, &NetworkRequestManager::genreResult, this, &SearchTab::handleGenresResult);

    // Connect the returnPressed signal of the searchLineEdit to executeSearch slot
    connect(searchLineEdit, &QLineEdit::returnPressed, this, &SearchTab::executeSearch);

    // Connect itemClicked signal to the gameClicked signal
    connect(resultListWidget, &QListWidget::itemClicked, this, [this](QListWidgetItem* item) {
        int index = resultListWidget->row(item); // Get the clicked item's index
        if (index >= 0 && index < requestManager.gameIds.size()) {
            int gameId = requestManager.gameIds[index];
            requestGameDetails(QString::number(gameId)); // Convert the gameId to QString if needed
        }
    });

    // Set placeholder text for the search input field
    searchLineEdit->setPlaceholderText("Search Game");

    // Widget layout (subject to change)
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(searchLineEdit);
    mainLayout->addWidget(resultListWidget);
    QHBoxLayout* layoutWithin = new QHBoxLayout;
    layoutWithin->addWidget(textBrowserWidget);
    layoutWithin->addWidget(imageLabel);
    mainLayout->addLayout(layoutWithin);
    mainLayout->addWidget(stopwatch);
    setLayout(mainLayout);
}

SearchTab::~SearchTab()
{
    delete searchLineEdit;
    delete resultListWidget;
    delete textBrowserWidget;
    delete imageLabel;
    delete stopwatch;
}

void SearchTab::executeSearch()
{
    // Get the search query from the QLineEdit
    QString searchQuery = searchLineEdit->text();

    // Send the search request using the NetworkRequestManager
    requestManager.sendSearchRequest(searchQuery);
}

void SearchTab::handleSearchResult(const QByteArray& result)
{

    // Parse the JSON response
    QJsonDocument jsonResponse = QJsonDocument::fromJson(result);

    if (!jsonResponse.isArray())
    {
        resultListWidget->addItem("Invalid JSON response.");
        return;
    }

    // Extract the names into the JSON array
    QJsonArray jsonArray = jsonResponse.array();
    QStringList namesList;

    // Gets the names and ids to display and send to details
    for (const QJsonValue& value : jsonArray)
    {
        if (value.isObject()) {
            QJsonObject obj = value.toObject();

            if (obj.contains("id"))
            {
                int gameId = obj["id"].toInt();
                requestManager.gameIds.append(gameId);
            }

            if (obj.contains("name"))
            {
                namesList.append(obj["name"].toString());
            }
        }
    }

    // Clear the existing list items
    resultListWidget->clear();

    if (namesList.isEmpty())
    {
        resultListWidget->addItem("No titles found.");
    }

    // Add the names and ids to the list widget
    for (int i = 0; i < namesList.size(); ++i)
    {
        QListWidgetItem* item = new QListWidgetItem(namesList[i]);
        item->setData(Qt::UserRole, requestManager.gameIds[i]); // Set the game id as custom data
        resultListWidget->addItem(item);
    }
    namesList.clear();
}

void SearchTab::handleSearchError(const QString& error)
{
    // Handle and display the search error
    resultListWidget->clear();
    resultListWidget->addItem("Error: " + error);
}

void SearchTab::requestGameDetails(const QString& gameId)
{
    QString gameRequest = QString("where id = %1; fields name, cover, parent_game, genres, age_ratings, "
                                  "platforms, first_release_date, franchise, franchises, "
                                  "involved_companies, summary, screenshots;").arg(gameId);

    // Send the game details request using the NetworkRequestManager
    requestManager.sendGameDetailsRequest(gameRequest);
}

void SearchTab::handleDetailsResult(const QByteArray& result)
{
    // Parse the JSON response
    QJsonDocument jsonResponse = QJsonDocument::fromJson(result);
    if (!jsonResponse.isArray())
    {
        resultListWidget->addItem("Invalid JSON response.");
        return;
    }

    // Extract the details from the JSON array
    QJsonArray jsonArray = jsonResponse.array();
    if (jsonArray.isEmpty())
    {
        resultListWidget->addItem("No data available.");
        return;
    }

    // Clear the existing content in the Text Browser
    textBrowserWidget->clear();

    // Iterate over the JSON array and append details to the Text Browser
    for (const QJsonValue& value : jsonArray)
    {
        if (value.isObject())
        {
            QJsonObject obj = value.toObject();

            QString name = obj["name"].toString();
            stopwatch->gameName = name;
            stopwatch->enable();

            if (obj["cover"].toInt())
            {
                qint64 cover_ID = obj["cover"].toInt();
                requestManager.handleCovers(cover_ID);
            } else {
                imageLabel->setText("No cover present");
            }

            qint64 unixTimestamp = obj["first_release_date"].toInt(); // Timestamp is in Unix Time
            QDateTime dateTime = QDateTime::fromSecsSinceEpoch(unixTimestamp);
            QString first_release_date = dateTime.toString("yyyy-MM-dd");

            QJsonArray platform_IDs = obj["platforms"].toArray();
            requestManager.handlePlatforms(platform_IDs);

            QJsonArray company_IDs = obj["involved_companies"].toArray();
            requestManager.handleInvolvedCompanies(company_IDs);

            //QString age_ratings = obj["age_ratings"].toString();

            QJsonArray genre_IDs = obj["genres"].toArray();
            requestManager.handleGenres(genre_IDs);

            QString summary = obj["summary"].toString();
            //screenshots here

            // Format the information and append it to the Text Browser
            if (!(platforms == QString())) // Double click check (only works once before changing operation)
            {
                QString formattedInfo = QString("%1\n\nPlatforms:\n%2\nOriginal Release Date:\n%3\n\n"
                                                "Companies:\n%4\nGenres:\n%5\nSummary:\n%6\n\n")
                                            .arg(name, platforms, first_release_date, companies, genres, summary);
                textBrowserWidget->append(formattedInfo);
                textBrowserWidget->verticalScrollBar()->setValue(0); // Set scroll bar to top

                platforms.clear();
                companies.clear();
                genres.clear();
            } else {
                textBrowserWidget->append("Click selected game title twice for complete details.");
                textBrowserWidget->append("(If this doesn't work, then there is likely "
                                          "no info for this game on IGDB. Feel free to add some!)");
            }
        }
    }
}

void SearchTab::handleCoverResult(const QByteArray& result)
{
    QJsonDocument jsonResponse = QJsonDocument::fromJson(result);
    QJsonArray jsonArray = jsonResponse.array();
    const QJsonValue& value = jsonArray.at(0);

    if (value.isObject())
    {
        QJsonObject obj = value.toObject();
        QString url(obj["url"].toString());
        url = QString("https:%1").arg(url);
        url.replace("t_thumb", "t_cover_med");

        QNetworkAccessManager* imageManager = new QNetworkAccessManager(this);
        connect(imageManager, &QNetworkAccessManager::finished, this, &SearchTab::handleImageDownloaded);
        QNetworkRequest imageRequest(url);
        imageManager->get(imageRequest);
    }
}

void SearchTab::handleImageDownloaded(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray imageData = reply->readAll();
        QPixmap pixmap;
        pixmap.loadFromData(imageData);

        imageLabel->setPixmap(pixmap);
    } else {
        imageLabel->setText("Image download failed");
    }

    reply->deleteLater();
}

void SearchTab::handlePlatformsResult(const QByteArray& result)
{
    QJsonDocument jsonResponse = QJsonDocument::fromJson(result);
    QJsonArray jsonArray = jsonResponse.array();
    const QJsonValue& value = jsonArray.at(0);

    if (value.isObject())
    {
        QJsonObject obj = value.toObject();
        platforms.append(obj["name"].toString());
        platforms.append("\n");
    }
}

void SearchTab::handleCompaniesResult(const QByteArray& result)
{
    QJsonDocument jsonResponse = QJsonDocument::fromJson(result);
    QJsonArray jsonArray = jsonResponse.array();
    const QJsonValue& value = jsonArray.at(0);

    if (value.isObject())
    {
        QJsonObject obj = value.toObject();
        companies.append(obj["name"].toString());
        companies.append("\n");
    }
}

void SearchTab::handleGenresResult(const QByteArray& result)
{
    QJsonDocument jsonResponse = QJsonDocument::fromJson(result);
    QJsonArray jsonArray = jsonResponse.array();
    const QJsonValue& value = jsonArray.at(0);

    if (value.isObject())
    {
        QJsonObject obj = value.toObject();
        genres.append(obj["name"].toString());
        genres.append("\n");
    }
}
