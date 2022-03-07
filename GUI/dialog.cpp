#include "dialog.h"
#include <QDebug>
#include <iostream>
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_buttonBox_accepted()
{
    QString username = ui->username->text();
    QString password = ui->password->text();
    ui->username->clear();
    ui->password->clear();
    emit Login(username, password);
}

