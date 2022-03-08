#ifndef CONTAINERBUTTON_H
#define CONTAINERBUTTON_H

#include <QCheckBox>

class ContainerButton : public QCheckBox
{
    Q_OBJECT;

public:
    ContainerButton(const QString&, int row, int col, QWidget* = nullptr);
    virtual void paintEvent(QPaintEvent* e) override;
signals:
    void containerClick(int, int);

public slots:
    void click();
//    void toggle();

private:
    int row;
    int col;
};

#endif // CONTAINERBUTTON_H
