#include "loadcontainer.h"
#include "ui_loadcontainer.h"

LoadContainer::LoadContainer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadContainer)
{
    ui->setupUi(this);
}

LoadContainer::~LoadContainer()
{
    delete ui;
}

void LoadContainer::on_pushButton_2_clicked()
{
    QString name = ui->lineEdit->text();
    QString weight = ui->lineEdit_2->text();

    if(name != "" && weight != "")
        emit Load(name, weight);

    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    this->close();
}

