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
    tabWidget->setStyleSheet("background-color: #F2F2FF;");
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

    setWindowTitle(tr("ROM-Record"));
    setWindowIcon(QIcon("./icon/icon.png"));
}

MainWindow::~MainWindow()
{
    delete tabWidget;
    delete ui;
}
