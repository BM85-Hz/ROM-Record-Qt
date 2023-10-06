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
    // Parse the JSON response
    QJsonDocument jsonResponse = QJsonDocument::fromJson(result);
    if (!jsonResponse.isArray()) {
        resultListWidget->addItem("Invalid JSON response.");
        return;
    }

    // Extract the names from the JSON array
    QJsonArray jsonArray = jsonResponse.array();
    QStringList namesList;
    for (const QJsonValue& value : jsonArray) {
        if (value.isObject()) {
            QJsonObject obj = value.toObject();
            if (obj.contains("name")) {
                namesList.append(obj["name"].toString());
            }
        }
    }

    // Clear the existing list items
    resultListWidget->clear();

    // Add the names to the list widget
    resultListWidget->addItems(namesList);
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
