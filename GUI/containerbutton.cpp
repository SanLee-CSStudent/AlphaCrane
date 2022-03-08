#include "containerbutton.h"
#include <QPainter>
#include <iostream>

ContainerButton::ContainerButton(const QString& innerText, int r, int c, QWidget* parent):
    QCheckBox(parent), row(r), col(c)
{
    setText(innerText);
    setCheckable(true);
    setChecked(false);
}

void ContainerButton::click(){
    QCheckBox::click();
    std::cout << "Clicked" << std::endl;
}

void ContainerButton::paintEvent(QPaintEvent* e){
    QCheckBox::paintEvent(e);
}
