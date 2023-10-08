#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QListWidget>
#include <QTextBrowser>
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
    void handleDetailsResult(const QByteArray& result);
    void handlePlatformsResult(const QByteArray& result);
    void handleCompaniesResult(const QByteArray& result);
    void handleGenresResult(const QByteArray& result);
    void executeSearch();
    void requestGameDetails(const QString& gameId);

private:
    Ui::MainWindow *ui;
    QLineEdit* searchLineEdit;
    QListWidget* resultListWidget;
    QTextBrowser* textBrowserWidget;
    NetworkRequestManager requestManager;

    QString platforms;
    QString companies;
    QString genres;
};
#endif // MAINWINDOW_H
