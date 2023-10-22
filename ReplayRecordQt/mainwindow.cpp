#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "searchtab.h"
#include "calendartab.h"
#include "liststab.h"
#include "abouttab.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tabWidget = new QTabWidget;
    tabWidget->setStyleSheet("background-color: white;");
    tabWidget->addTab(new SearchTab, tr("Search"));
    tabWidget->addTab(new CalendarTab, tr("Calendar"));
    tabWidget->addTab(new ListsTab, tr("Lists"));
    tabWidget->addTab(new AboutTab, tr("About"));

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    QWidget* centralWidget = new QWidget;
    centralWidget->setLayout(mainLayout);
    centralWidget->setStyleSheet("background-color: #E9FAFF;");
    setCentralWidget(centralWidget);

    setWindowTitle(tr("ReplayRecord"));
    setWindowIcon(QIcon("icon.ico"));
}

MainWindow::~MainWindow()
{
    delete tabWidget;
    delete ui;
}
