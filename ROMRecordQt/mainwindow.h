#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QListWidget>
#include <QVBoxLayout>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "networkrequestmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleSearchResult(const QByteArray& result);
    void handleSearchError(const QString& error);
    void executeSearch();
    void requestGameDetails(const QString& gameId);

private:
    Ui::MainWindow *ui;
    QLineEdit* searchLineEdit;
    QListWidget* resultListWidget;
    NetworkRequestManager requestManager;
};
#endif // MAINWINDOW_H
