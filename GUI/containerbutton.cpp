#include "containerbutton.h"
#include <QStylePainter>
#include <QDebug>
#include <QObject>
#include <iostream>

ContainerButton::ContainerButton(const QString& innerText, int r, int c, STATE state, QWidget* parent):
    QPushButton(parent), row(r), col(c), state(state)
{
    setText(innerText);
    setCheckable(true);
    setChecked(false);
    connect(this, &QPushButton::clicked, this, &ContainerButton::click);
}

void ContainerButton::setState(STATE newState){
    state = newState;
}
void ContainerButton::click(bool checked){
    if((this->state == NOTAVAIL) || (this->state == MOVING) || (this->state == EMPTY))
        return;
    if(checked){
        this->state = SELECTED;
        emit containerSelected(this);
    }else{
        this->state = OCCUPIED;
        emit containerDeselected(this);
    }
}

void ContainerButton::paintEvent(QPaintEvent* e){
    QColor color;
    switch(state){
        case SELECTED:
            color = QColor("red");
            break;
        case NOTAVAIL:
            color = QColor("grey");
            break;
        case EMPTY:
            color = QColor("white");
            break;
        case MOVING:
            color = QColor("green");
            break;
        case OCCUPIED:
            color = QColor("lightblue");
            break;
    }
    QStylePainter painter(this);
    painter.fillRect(rect(), color);
    painter.drawText(rect(), Qt::AlignCenter, text());
}
int ContainerButton::getRow() const {
    return row;
}
int ContainerButton::getCol() const {
    return col;
}

