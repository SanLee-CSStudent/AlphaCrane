#include "containerbutton.h"
#include <QStylePainter>
#include <QDebug>
#include <QObject>

const QColor ContainerButton::colors[] =
{
        QColor("red"),
        QColor("grey"),
        QColor("lightblue"),
        QColor("white"),
        QColor("green")
};

ContainerButton::ContainerButton(const QString& innerText, int r, int c, STATE state, QWidget* parent):
    QPushButton(parent), row(r), col(c), state(state)
{
    setText(innerText);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    setCheckable(true);
    setChecked(false);
    this->color = colors[state];

    connect(this, &QPushButton::clicked, this, &ContainerButton::click);
}

void ContainerButton::setState(STATE newState){
    state = newState;
    this->color = colors[state];
}

ContainerButton::STATE ContainerButton::getState() const {
    return state;
}

void ContainerButton::click(bool checked){
    if((this->state == ContainerButton::NOTAVAIL) ||
       (this->state == ContainerButton::MOVING)   ||
       (this->state == ContainerButton::EMPTY))
        return;

    if(checked)
    {
        setState(ContainerButton::SELECTED);
        emit containerSelected(this);
    }
    else
    {

        setState(ContainerButton::OCCUPIED);
        emit containerDeselected(this);
    }
    repaint();
}

void ContainerButton::paintEvent(QPaintEvent* e){
//    QColor color;
    QStylePainter painter(this);
    painter.fillRect(rect(), this->color);
    painter.drawText(rect(), Qt::AlignCenter, text());
}
int ContainerButton::getRow() const {
    return row;
}
int ContainerButton::getCol() const {
    return col;
}

