#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "dialog.h"
#include <QFileInfo>
#include <QDebug>
#include <QObject>
#include <sstream>
#include <iostream>
#include "containerbutton.h"
#define SHIP_ROWS 8
#define SHIP_COLS 12
#define BUFF_ROWS 4
#define BUFF_COLS 24

void makeContainerGrid(QWidget *grid, int rows, int cols, const QSize buttonSize){
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            ContainerButton* currentContainer =
                    new ContainerButton(QString::number(c) + " " + QString::number(r), r, c, grid);
            currentContainer->setFixedSize(buttonSize);
            QGridLayout* layout = (QGridLayout*) grid->layout();
            layout->addWidget(currentContainer, r, c);
        }
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
      ui->setupUi(this);
      this->setFixedSize(1200, 900);
      makeContainerGrid(ui->shipWidget, SHIP_ROWS, SHIP_COLS, QSize(80,80));
      makeContainerGrid(ui->bufferWidget, BUFF_ROWS, BUFF_COLS, QSize(40, 40));
      on_actionLogin_triggered();
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
    ui->actionCreate_annotation->setEnabled(true);
}
void MainWindow::Logout(){
    ui->actionImport_Manifest->setEnabled(false);
    ui->actionLogout->setEnabled(false);
    ui->actionLogin->setEnabled(true);
    ui->actionCreate_annotation->setEnabled(false);
}
void MainWindow::acceptLoginDialog(const QString& username, const QString& password){
    std::cout << username.toStdString() << password.toStdString() << std::endl;
    if(username == "Admin" && password == "password"){
        Login();
    }
}

void MainWindow::acceptAnnotateDialog(const QString& annotation){
    std::cout << "From main window" << annotation.toStdString() << std::endl;
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
    if(loggin == nullptr){
        loggin = new Dialog(this);
        loggin->setModal(true);
    }
    loggin->exec();
    connect(loggin,
            &Dialog::Login,
            this,
            &MainWindow::acceptLoginDialog);
}


void MainWindow::on_actionLogout_triggered()
{
    Logout();
}


void MainWindow::on_actionCreate_annotation_triggered()
{
    if(annotation == nullptr){
        annotation = new Annotation(this);
        annotation->setModal(true);
    }
    annotation->exec();
    connect(annotation,
            &Annotation::acceptAnnotation,
            this,
            &MainWindow::acceptAnnotateDialog);
}

