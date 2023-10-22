#ifndef ABOUTTAB_H
#define ABOUTTAB_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class AboutTab : public QWidget
{
public:
    AboutTab(QWidget *parent = nullptr);
    ~AboutTab();
private:
    QLabel* topText;
    QLabel* imageLabel;
    QLabel* bottomText;
};

#endif // ABOUTTAB_H
