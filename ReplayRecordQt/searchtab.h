#ifndef SEARCHTAB_H
#define SEARCHTAB_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QListWidget>
#include <QTextBrowser>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QPixmap>
#include <QLabel>
#include <QTabWidget>
#include "networkrequestmanager.h"
#include "stopwatch.h"

class SearchTab : public QWidget
{
    Q_OBJECT
public:
    explicit SearchTab(QWidget *parent = nullptr);
    ~SearchTab();

private slots:
    void executeSearch();
    void handleSearchResult(const QByteArray& result);
    void handleSearchError(const QString& error);
    void handleDetailsResult(const QByteArray& result);
    void handleCoverResult(const QByteArray& result);
    void handleImageDownloaded(QNetworkReply* reply);
    void handlePlatformsResult(const QByteArray& result);
    void handleCompaniesResult(const QByteArray& result);
    void handleGenresResult(const QByteArray& result);
    void requestGameDetails(const QString& gameId);

private:
    QLineEdit* searchLineEdit;
    QListWidget* resultListWidget;
    QTextBrowser* textBrowserWidget;
    Stopwatch* stopwatch;
    NetworkRequestManager requestManager;

    QLabel* imageLabel{};
    QString platforms{};
    QString companies{};
    QString genres{};
    QString name{};
    QString nameBuf{};
    QString firstReleaseDate{};
    QString firstReleaseDateBuf{};
    QString summary{};
    QString summaryBuf{};
    QPixmap pixmap{};
    QPixmap pixmapBuf{};
};

#endif // SEARCHTAB_H
