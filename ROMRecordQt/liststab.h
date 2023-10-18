#ifndef LISTSTAB_H
#define LISTSTAB_H

#include <QObject>
#include <QWidget>
#include <QFileSystemWatcher>
#include <QTextBrowser>
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
    QString loadRecents();
    QString loadTotals();

private:
    QTextBrowser* recentlyPlayed;
    QLabel* recentLabel;
    QTextBrowser* totalAmounts;
    QLabel* totalLabel;
};

#endif // LISTSTAB_H
