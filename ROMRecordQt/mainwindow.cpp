#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "searchtab.h"
#include "calendartab.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tabWidget = new QTabWidget;
    tabWidget->addTab(new SearchTab, tr("Search"));
    tabWidget->addTab(new CalendarTab, "Calendar");

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    QWidget* centralWidget = new QWidget;
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    setWindowTitle(tr("ROM-Record"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
