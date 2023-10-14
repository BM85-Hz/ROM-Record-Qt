#include "stopwatch.h"

Stopwatch::Stopwatch(QWidget *parent)
    : QWidget(parent), elapsedTime(0), isRunning(false)
{
    // Create and configure the UI components
    timerLabel = new QLabel("00:00:00");
    buttonStack = new QStackedWidget;

    startButton = new QPushButton("Start");
    stopButton = new QPushButton("Stop");
    discardButton = new QPushButton("Discard");
    resumeButton = new QPushButton("Resume");
    saveButton = new QPushButton("Save");

    // Add buttons to the stacked widget
    buttonStack->addWidget(startButton);
    buttonStack->addWidget(stopButton);
    QHBoxLayout* optionButtonsLayout = new QHBoxLayout;
    QWidget* optionButtons = new QWidget;
    optionButtonsLayout->addWidget(discardButton);
    optionButtonsLayout->addWidget(resumeButton);
    optionButtonsLayout->addWidget(saveButton);
    optionButtons->setLayout(optionButtonsLayout);
    buttonStack->addWidget(optionButtons);

    // Connect the button clicks to slots
    connect(startButton, &QPushButton::clicked, this, &Stopwatch::start);
    connect(stopButton, &QPushButton::clicked, this, &Stopwatch::stop);
    connect(discardButton, &QPushButton::clicked, this, &Stopwatch::discard);
    connect(resumeButton, &QPushButton::clicked, this, &Stopwatch::resume);

    // Create and configure layout
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(timerLabel);
    layout->addWidget(buttonStack);
    setLayout(layout);

    // Create a timer to update the stopwatch display
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Stopwatch::updateTime);
    timer->start(1000); // Update the time every second
}

void Stopwatch::start(){
    if (!isRunning) {
        isRunning = true;
        startTime = QDateTime::currentDateTime().addMSecs(-elapsedTime);
        buttonStack->setCurrentIndex(1); // Show the Stop button
        updateTime(); // Update the timer label
    }
}

void Stopwatch::stop(){
    if (isRunning) {
        isRunning = false;
        elapsedTime = startTime.msecsTo(QDateTime::currentDateTime());
        buttonStack->setCurrentIndex(2); // Show the Discard, Resume, Save buttons
    }
}

void Stopwatch::discard() {
    if (!isRunning) {
        elapsedTime = 0;
        updateTime();
        buttonStack->setCurrentIndex(0); // Show the Start button
    }
}

void Stopwatch::resume() {
    if (!isRunning) {
        start();
    }
}

void Stopwatch::updateTime() {
    if (isRunning) {
        int elapsedSeconds = elapsedTime / 1000;
        int hours = elapsedSeconds / 3600;
        int minutes = (elapsedSeconds % 3600) / 60;
        int seconds = elapsedSeconds % 60;
        timerLabel->setText(QString("%1:%2:%3").arg(hours, 2, 10, QChar('0')).arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));
    }
}
