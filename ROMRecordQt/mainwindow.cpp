#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    searchLineEdit = new QLineEdit(this);
    resultTextEdit = new QPlainTextEdit(this);
    setCentralWidget(resultTextEdit);

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
    layout->addWidget(resultTextEdit);
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
    // Display search results in the text widget
    resultTextEdit->setPlainText(result);
}

void MainWindow::handleSearchError(const QString& error)
{
    // Handle and display the search error
    resultTextEdit->setPlainText("Error: " + error);
}

void MainWindow::executeSearch()
{
    // Get the search query from the QLineEdit
    QString searchQuery = searchLineEdit->text();

    // Send the search request using the NetworkRequestManager
    requestManager.sendSearchRequest(searchQuery);
}
