#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    searchLineEdit = new QLineEdit(this);
    resultListWidget = new QListWidget(this);
    textBrowserWidget = new QTextBrowser(this);
    setCentralWidget(resultListWidget);

    // Connect signals and slots
    connect(&requestManager, &NetworkRequestManager::searchResult, this, &MainWindow::handleSearchResult);
    connect(&requestManager, &NetworkRequestManager::searchError, this, &MainWindow::handleSearchError);
    connect(&requestManager, &NetworkRequestManager::gameDetailsResult, this, &MainWindow::handleDetailsResult);
    connect(&requestManager, &NetworkRequestManager::platformResult, this, &MainWindow::handlePlatformsResult);
    connect(&requestManager, &NetworkRequestManager::companyResult, this, &MainWindow::handleCompaniesResult);

    // Connect the returnPressed signal of the searchLineEdit to executeSearch slot
    connect(searchLineEdit, &QLineEdit::returnPressed, this, &MainWindow::executeSearch);

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
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(searchLineEdit);
    layout->addWidget(resultListWidget);
    layout->addWidget(textBrowserWidget);
    QWidget* centralWidget = new QWidget;
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleSearchResult(const QByteArray& result)
{

    qDebug() << "Received JSON response:" << result;

    // Parse the JSON response
    QJsonDocument jsonResponse = QJsonDocument::fromJson(result);
    if (!jsonResponse.isArray()) {
        resultListWidget->addItem("Invalid JSON response.");
        return;
    }

    // Extract the names into the JSON array
    QJsonArray jsonArray = jsonResponse.array();
    QStringList namesList;
    for (const QJsonValue& value : jsonArray) {
        if (value.isObject()) {
            QJsonObject obj = value.toObject();

            if (obj.contains("id")) {
                int gameId = obj["id"].toInt();
                requestManager.gameIds.append(gameId);
                //qDebug() << gameId;
            }

            if (obj.contains("name")) {
                namesList.append(obj["name"].toString());
                //qDebug() << obj["name"].toString();
            }
        }
    }

    // Clear the existing list items
    resultListWidget->clear();

    // Add the names and ids to the list widget
    for (int i = 0; i < namesList.size(); ++i) {
        QListWidgetItem* item = new QListWidgetItem(namesList[i]);
        item->setData(Qt::UserRole, requestManager.gameIds[i]); // Set the game id as custom data
        resultListWidget->addItem(item);
    }
}

void MainWindow::handleSearchError(const QString& error)
{
    // Handle and display the search error
    resultListWidget->clear();
    resultListWidget->addItem("Error: " + error);
}

void MainWindow::executeSearch()
{
    // Get the search query from the QLineEdit
    QString searchQuery = searchLineEdit->text();

    // Send the search request using the NetworkRequestManager
    requestManager.sendSearchRequest(searchQuery);
}

void MainWindow::requestGameDetails(const QString& gameId)
{
    QString gameRequest = QString("where id = %1; fields name, cover, parent_game, genres, age_ratings, platforms, first_release_date, franchise, franchises, involved_companies, summary, screenshots;").arg(gameId);

    // Send the game details request using the NetworkRequestManager
    requestManager.sendGameDetailsRequest(gameRequest);
}

void MainWindow::handleDetailsResult(const QByteArray& result)
{
    qDebug() << "Received JSON response:" << result;

    // Parse the JSON response
    QJsonDocument jsonResponse = QJsonDocument::fromJson(result);
    if (!jsonResponse.isArray()) {
        resultListWidget->addItem("Invalid JSON response.");
        return;
    }

    // Extract the details from the JSON array
    QJsonArray jsonArray = jsonResponse.array();
    if (jsonArray.isEmpty()) {
        resultListWidget->addItem("No data available.");
        return;
    }

    // Clear the existing content in the Text Browser
    textBrowserWidget->clear();

    // Iterate over the JSON array and append details to the Text Browser
    for (const QJsonValue& value : jsonArray) {
        if (value.isObject()) {
            QJsonObject obj = value.toObject();

            QString name = obj["name"].toString();
            //add cover here
            qint64 unixTimestamp = obj["first_release_date"].toInt(); // Timestamp is in Unix Time
                QDateTime dateTime = QDateTime::fromSecsSinceEpoch(unixTimestamp);
                QString first_release_date = dateTime.toString("yyyy-MM-dd");

            QJsonArray platform_IDs = obj["platforms"].toArray();
                requestManager.handlePlatforms(platform_IDs);

            QJsonArray company_IDs = obj["involved_companies"].toArray();
                qDebug() << company_IDs;
                requestManager.handleInvolvedCompanies(company_IDs);

            QString age_ratings = obj["age_ratings"].toString();
            QString genres = obj["genres"].toString();
            QString summary = obj["summary"].toString();
            //screenshots here

            // Format the information and append it to the Text Browser
            if (!platforms.isEmpty()){
                QString formattedInfo = QString("%1\n\nPlatforms:\n%2\nOriginal Release Date: %3\n\nCompanies:\n%4\n"
                                                "Age Ratings: %5\n\nGenres: %6\n\nSummary:\n%7\n\n")
                                            .arg(name, platforms, first_release_date, companies, age_ratings, genres, summary);
                textBrowserWidget->append(formattedInfo);
                platforms.clear();
                companies.clear();
            }
        }
    }
}

void MainWindow::handlePlatformsResult(const QByteArray& result){
    QJsonDocument jsonResponse = QJsonDocument::fromJson(result);
    QJsonArray jsonArray = jsonResponse.array();
    const QJsonValue& value = jsonArray.at(0);
    if (value.isObject()) {
        QJsonObject obj = value.toObject();
        platforms.append(obj["name"].toString());
        platforms.append("\n");
    }
}

void MainWindow::handleCompaniesResult(const QByteArray& result){
    QJsonDocument jsonResponse = QJsonDocument::fromJson(result);
    QJsonArray jsonArray = jsonResponse.array();
    const QJsonValue& value = jsonArray.at(0);
    if (value.isObject()) {
        QJsonObject obj = value.toObject();
        companies.append(obj["name"].toString());
        companies.append("\n");
    }
}
