#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QDateTime>
#include <QVBoxLayout>
#include <QTimer>
#include <QDialogButtonBox>
#include <QStackedWidget>

class Stopwatch : public QWidget
{
    Q_OBJECT
public:
    Stopwatch(QWidget *parent = nullptr);
    ~Stopwatch(){}

public slots:
    void start();
    void stop();
    void discard();
    void resume();
    void updateTime();

private:
    QLabel* timerLabel;
    QDialogButtonBox* buttonBox;
    QStackedWidget* buttonStack;
    QPushButton* startButton;
    QPushButton* stopButton;
    QPushButton* discardButton;
    QPushButton* resumeButton;
    QPushButton* saveButton;
    QDateTime startTime;
    qint64 elapsedTime;
    bool isRunning;
};

#endif // STOPWATCH_H
