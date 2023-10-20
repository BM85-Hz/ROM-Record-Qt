#include "stopwatch.h"

Stopwatch::Stopwatch(QWidget *parent)
    : QWidget(parent), gameName(""), elapsedTime(0), isRunning(false)
{
    // Create and configure the UI components
    timerLabel = new QLabel("00:00:00");
    currentSessionLabel = new QLabel("Currently Playing: Nothing Yet!");
    buttonStack = new QStackedWidget;
    logmaker = new LogMaker;

    startButton = new QPushButton("Start");
    pauseButton = new QPushButton("Stop");
    pauseButton->setStyleSheet("background-color: #FFD7D7;");
    discardButton = new QPushButton("Discard");
    discardButton->setStyleSheet("background-color: #EBE4FF;");
    resumeButton = new QPushButton("Resume");
    resumeButton->setStyleSheet("background-color: #E6FFE4;");
    saveButton = new QPushButton("Save");
    saveButton->setStyleSheet("background-color: #EBE4FF;");

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
    connect(saveButton, &QPushButton::clicked, this, &Stopwatch::save);

    // Create and configure layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(timerLabel);
    mainLayout->addWidget(currentSessionLabel);
    mainLayout->addWidget(buttonStack);
    setLayout(mainLayout);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Stopwatch::updateTime);

    startButton->setEnabled(false);
}

Stopwatch::~Stopwatch()
{
    delete timerLabel;
    delete currentSessionLabel;
    delete startButton;
    delete pauseButton;
    delete discardButton;
    delete resumeButton;
    delete saveButton;
    delete buttonStack;
    delete logmaker;
    delete timer;
}

void Stopwatch::enable()
{
    if(!gameName.isNull())
        startButton->setEnabled(true);
}

void Stopwatch::start()
{
    if (!isRunning) {
        startDate = QDate::currentDate();
        startTime = QDateTime::currentDateTime();
        isRunning = true;
        currentSession = gameName;
        QString text{QString("Currently Playing: %1").arg(currentSession)};
        currentSessionLabel->setText(text);
    }
    buttonStack->setCurrentIndex(1); // Show the Stop button
    timer->start(1); // Iterates by millisecond
}

// Called stop in the program
void Stopwatch::pause()
{
    if (isRunning) {
        isRunning = false;
        buttonStack->setCurrentIndex(2); // Show the Discard, Resume, Save buttons
        timer->stop();
    }
}

void Stopwatch::discard()
{
    if (!isRunning) {
        timerLabel->setText("00:00:00");
        currentSessionLabel->setText("Currently Playing: Nothing at the Moment");
        buttonStack->setCurrentIndex(0); // Show the Start button
    }
}

void Stopwatch::resume()
{
    if (!isRunning) {
        isRunning = true;

        // Makes sure time actually resumes from the pause point
        startTime = QDateTime::currentDateTime().addSecs(-elapsedTime / 1000);
        start();
    }
}

void Stopwatch::save()
{
    if(!isRunning){
        logmaker->saveToLogs(currentSession, elapsedTime, startDate);
        timerLabel->setText("00:00:00");
        currentSessionLabel->setText("Previous session added to logs.");
        buttonStack->setCurrentIndex(0); // Show the Start button
    }
}

// Updates the time label on screen
void Stopwatch::updateTime()
{
    if (isRunning) {
        elapsedTime = startTime.msecsTo(QDateTime::currentDateTime());
        int h = elapsedTime / 1000 / 60 / 60;
        int m = (elapsedTime / 1000 / 60) % 60;
        int s = (elapsedTime / 1000) % 60;
        //int ms = elapsedTime - (s * 1000) - (m * 60000) - (h * 3600000);

        // Args take the variable, fieldwidth, base (decimal), and character (a '0'), in that order
        QString diff{QString("%1:%2:%3").
                       arg(h, 2, 10, zero).
                       arg(m, 2, 10, zero).
                       arg(s, 2, 10, zero)};
                       //arg(ms, 3, 10, zero);
        timerLabel->setText(diff);
    }
}
