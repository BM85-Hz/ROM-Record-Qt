#include "abouttab.h"

AboutTab::AboutTab(QWidget *parent)
    : QWidget(parent)
{
    topText = new QLabel("ReplayRecord v1.0.3");
    topText->setAlignment(Qt::AlignCenter);
    topText->setStyleSheet("font-size: 16pt; font-weight: bold;");

    imageLabel = new QLabel;
    QPixmap pixmap;
    pixmap.load("./icon/icon.png");
    imageLabel->setPixmap(pixmap);
    imageLabel->setAlignment(Qt::AlignCenter);

    bottomText = new QLabel("A general purpose game logger programmed in C++, "
                            "based on Qt toolkit\nCopyright © 2023 Hz-Voyager");
    bottomText->setAlignment(Qt::AlignCenter);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(topText);
    mainLayout->addWidget(imageLabel);
    mainLayout->addWidget(bottomText);
    setLayout(mainLayout);
}

AboutTab::~AboutTab()
{
    delete topText;
    delete imageLabel;
    delete bottomText;
}
