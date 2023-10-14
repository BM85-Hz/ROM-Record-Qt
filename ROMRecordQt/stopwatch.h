#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTime>
#include <QVBoxLayout>
#include <QTimer>
#include <QStackedWidget>

class Stopwatch : public QWidget
{
    Q_OBJECT
public:
    Stopwatch(QWidget *parent = nullptr);
    ~Stopwatch(){}
    QString gameName;

public slots:
    void enable();
    void disable();
    void start();
    void pause();
    void discard();
    void resume();
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
    QTime startTime;
    qint64 elapsedTime;
    bool isRunning;
    const QChar zero{'0'};
};

#endif // STOPWATCH_H
