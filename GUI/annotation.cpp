#include "annotation.h"
#include "ui_annotation.h"
#include <iostream>

Annotation::Annotation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Annotation)
{
    ui->setupUi(this);
}

Annotation::~Annotation()
{
    delete ui;
}

void Annotation::on_pushButton_clicked()
{
    QString text = ui->textEdit->toPlainText();
    std::cout << text.toStdString() << std::endl;
    emit acceptAnnotation(text);
    this->close();
}

