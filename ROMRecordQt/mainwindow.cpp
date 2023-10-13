#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "searchtab.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tabWidget = new QTabWidget;
    QWidget* calendarTab = new QWidget();
    tabWidget->addTab(new SearchTab, tr("Search"));
    tabWidget->addTab(calendarTab, "Calendar");

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    QWidget* centralWidget = new QWidget;
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
