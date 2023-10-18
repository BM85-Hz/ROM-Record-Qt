#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QDateTime>
#include <QVBoxLayout>
#include <QTimer>
#include <QStackedWidget>
#include "logmaker.h"

class Stopwatch : public QWidget
{
    Q_OBJECT
public:
    explicit Stopwatch(QWidget *parent = nullptr);
    ~Stopwatch();
    QString gameName;

public slots:
    void enable();
    void start();
    void pause();
    void discard();
    void resume();
    void save();
    void updateTime();

private:
    QLabel* timerLabel;
    QTimer* timer;
    QStackedWidget* buttonStack;
    QPushButton* startButton;
    QPushButton* pauseButton;
    QPushButton* discardButton;
    QPushButton* resumeButton;
    QPushButton* saveButton;
    QDate startDate;
    QDateTime startTime;
    qint64 elapsedTime;
    bool isRunning;
    const QChar zero{'0'};

    QString currentSession;
    QLabel* currentSessionLabel;

    LogMaker* logmaker;
};

#endif // STOPWATCH_H
