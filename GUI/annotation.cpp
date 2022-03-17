#include "annotation.h"
#include "ui_annotation.h"

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
    const QString& text = ui->textEdit->toPlainText();
    emit acceptAnnotation(text);
    ui->textEdit->clear();
    this->close();
}

