#ifndef CONTAINERBUTTON_H
#define CONTAINERBUTTON_H

#include <QPushButton>
#include <QGridLayout>
#include <QColor>
#include <QObject>

class ContainerButton : public QPushButton
{
    Q_OBJECT;

public:
    enum STATE {SELECTED, NOTAVAIL, OCCUPIED, EMPTY, MOVING};
    const static QColor colors[];
    ContainerButton(const QString&, int row, int col, STATE state, QWidget* = nullptr);
    int getRow() const;
    int getCol() const;
    void setState(STATE newState);
    STATE getState() const;
signals:
    void containerSelected(const ContainerButton* button);
    void containerDeselected(const ContainerButton* button);
public slots:
    void click(bool checked);
    void paintEvent(QPaintEvent* e);
private:
    int row;
    int col;
    QColor color;
    STATE state;
};

#endif // CONTAINERBUTTON_H
