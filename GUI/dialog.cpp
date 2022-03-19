#include <QDebug>
#include <iostream>
#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
//    this->setFixedSize(QSize(200, 200));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_buttonBox_accepted()
{
    username = ui->username->text();
    QString password = ui->password->text();
    ui->username->clear();
    ui->password->clear();
    emit Login(username, password);
}

