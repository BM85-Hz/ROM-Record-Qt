#include "stopwatch.h"

Stopwatch::Stopwatch(QWidget *parent)
    : QWidget(parent), gameName(""), elapsedTime(0), isRunning(false)
{
    // Create and configure the UI components
    timerLabel = new QLabel("00:00:00");
    buttonStack = new QStackedWidget;

    startButton = new QPushButton("Start");
    pauseButton = new QPushButton("Stop");
    discardButton = new QPushButton("Discard");
    resumeButton = new QPushButton("Resume");
    saveButton = new QPushButton("Save");

    // Add buttons to the stacked widget
    buttonStack->addWidget(startButton);
    buttonStack->addWidget(pauseButton);
    QHBoxLayout* optionButtonsLayout = new QHBoxLayout;
    QWidget* optionButtons = new QWidget;
    optionButtonsLayout->addWidget(discardButton);
    optionButtonsLayout->addWidget(resumeButton);
    optionButtonsLayout->addWidget(saveButton);
    optionButtons->setLayout(optionButtonsLayout);
    buttonStack->addWidget(optionButtons);

    // Connect the button clicks to slots
    connect(startButton, &QPushButton::clicked, this, &Stopwatch::start);
    connect(pauseButton, &QPushButton::clicked, this, &Stopwatch::pause);
    connect(discardButton, &QPushButton::clicked, this, &Stopwatch::discard);
    connect(resumeButton, &QPushButton::clicked, this, &Stopwatch::resume);

    // Create and configure layout
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(timerLabel);
    layout->addWidget(buttonStack);
    setLayout(layout);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Stopwatch::updateTime);
    startTimer(0);

    startButton->setEnabled(false);
}

void Stopwatch::enable(){
    if(!gameName.isNull())
        startButton->setEnabled(true);
}

void Stopwatch::disable(){
    if(gameName.isEmpty())
        startButton->setEnabled(false);
}

void Stopwatch::start(){
    if (!isRunning) {
        startTime = QTime::currentTime();
        isRunning = true;
    }
    elapsedTime = 0;
    buttonStack->setCurrentIndex(1); // Show the Stop button
    timer->start(1);
}

void Stopwatch::pause(){
    if (isRunning) {
        isRunning = false;
        buttonStack->setCurrentIndex(2); // Show the Discard, Resume, Save buttons
        timer->stop();
        //qDebug() << elapsedTime;
    }
}

void Stopwatch::discard() {
    if (!isRunning) {
        timerLabel->setText("00:00:00");
        buttonStack->setCurrentIndex(0); // Show the Start button
    }
}

void Stopwatch::resume() {
    if (!isRunning) {
        isRunning = true;
        startTime = QTime::currentTime().addSecs(-elapsedTime / 1000);
        start();
    }
}

void Stopwatch::updateTime() {
    if (isRunning) {
        elapsedTime = startTime.msecsTo(QTime::currentTime());
        int h = elapsedTime / 1000 / 60 / 60;
        int m = (elapsedTime / 1000 / 60) - (h * 60);
        int s = (elapsedTime / 1000) - (m * 60);
        //int ms = elapsedTime - (s * 1000) - (m * 60000) - (h * 3600000);
        QString diff = QString("%1:%2:%3").
                       arg(h, 2, 10, zero).
                       arg(m, 2, 10, zero).
                       arg(s, 2, 10, zero);
                       //arg(ms, 3, 10, zero);
        timerLabel->setText(diff);
    }
}
