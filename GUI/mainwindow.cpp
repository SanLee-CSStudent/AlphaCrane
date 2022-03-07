#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "dialog.h"
#include <QFileInfo>
#include <QDebug>
#include <QObject>
#include <sstream>
#include <iostream>

#define SHIP_ROWS 8
#define SHIP_COLS 12
#define BUFF_ROWS 4
#define BUFF_COLS 24

void makeContainerGrid(QGridLayout *grid, int rows, int cols, const QSize buttonSize){
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            QPushButton* currentContainer = new QPushButton(QString::number(c) + " " + QString::number(r));
            currentContainer->setFixedSize(buttonSize);
            grid->addWidget(currentContainer, r, c);
        }
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
      ui->setupUi(this);
      loggin = new Dialog(this);
      this->setFixedSize(1200, 900);
      makeContainerGrid((QGridLayout*) ui->shipWidget->layout(), SHIP_ROWS, SHIP_COLS, QSize(80,80));
      makeContainerGrid((QGridLayout*) ui->bufferWidget->layout(), BUFF_ROWS, BUFF_COLS, QSize(40, 40));
      connect(loggin,
              &Dialog::Login,
              this,
              &MainWindow::acceptLoginDialog);

}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_nextButton_clicked()
{
    std::cout << current_step << std::endl;
    current_step += 1;
    if(current_step == steps.size() - 1) {
        ui->nextButton->setDisabled(true);
        ui->doneButton->setEnabled(true);
    }
}
void MainWindow::Login(){
    ui->actionImport_Manifest->setEnabled(true);
    ui->actionLogout->setEnabled(true);
    ui->actionLogin->setEnabled(false);
}
void MainWindow::Logout(){
    ui->actionImport_Manifest->setEnabled(false);
    ui->actionLogout->setEnabled(false);
    ui->actionLogin->setEnabled(true);
}
void MainWindow::acceptLoginDialog(const QString& username, const QString& password){
    std::cout << username.toStdString() << password.toStdString() << std::endl;
    if(username == "Admin" && password == "password"){
        Login();
    }
}

void MainWindow::on_actionImport_Manifest_triggered()
{
    QString next_file = QFileDialog::getOpenFileName(this, "Import the manifest");
    if(next_file != ""){
        QFile file(next_file);
        QFileInfo file_info(file);
        ui->label->setText(file_info.baseName());
    }else{
        ui->label->setText(QString("No manifest imported"));
    }
}


void MainWindow::on_doneButton_clicked()
{
    QMessageBox::information(this, "REMINDER", "Please send manifest to ship!");
}


void MainWindow::on_actionLogin_triggered()
{
    loggin->setModal(true);
    loggin->exec();
}


void MainWindow::on_actionLogout_triggered()
{
    Logout();
}

