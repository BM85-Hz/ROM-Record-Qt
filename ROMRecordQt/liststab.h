#ifndef LISTSTAB_H
#define LISTSTAB_H

#include <QObject>
#include <QWidget>
#include <QFileSystemWatcher>
#include <QTextBrowser>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVBoxLayout>
#include <QLabel>
#include <QString>

class ListsTab : public QWidget
{
    Q_OBJECT
public:
    explicit ListsTab(QWidget* parent = nullptr);
    ~ListsTab();

private slots:
    void updateBrowsers();
    QString loadRecents();
    QString loadTotals();

private:
    QTextBrowser* recentlyPlayed;
    QLabel* recentLabel;
    QTextBrowser* totalAmounts;
    QLabel* totalLabel;
};

#endif // LISTSTAB_H
