#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    searchLineEdit = new QLineEdit(this);
    resultListWidget = new QListWidget(this);
    setCentralWidget(resultListWidget);

    // Connect signals and slots
    connect(&requestManager, &NetworkRequestManager::searchResult, this, &MainWindow::handleSearchResult);
    connect(&requestManager, &NetworkRequestManager::searchError, this, &MainWindow::handleSearchError);

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
                qDebug() << gameId;
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
