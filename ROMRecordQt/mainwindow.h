#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPlainTextEdit>
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

private:
    Ui::MainWindow *ui;
    QLineEdit* searchLineEdit;
    QPlainTextEdit* resultTextEdit;
    NetworkRequestManager requestManager;

private slots:
    void handleSearchResult(const QByteArray& result);
    void handleSearchError(const QString& error);
    void executeSearch();
};
#endif // MAINWINDOW_H
