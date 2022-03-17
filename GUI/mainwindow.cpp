#include <QFileInfo>
#include <QFile>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QObject>

#include "containerbutton.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "dialog.h"

#define SHIP_ROWS 8
#define SHIP_COLS 12
#define BUFF_ROWS 4
#define BUFF_COLS 24

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
      ui->setupUi(this);
      qDebug() << "Something" << '\n';
      this->setFixedSize(1200, 900);
      makeContainerGrid(ui->shipWidget, SHIP_ROWS, SHIP_COLS, QSize(82,82));
      makeContainerGrid(ui->bufferWidget, BUFF_ROWS, BUFF_COLS, QSize(40, 40));
      on_actionLogin_triggered();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::makeContainerGrid(QWidget *grid, int rows, int cols, const QSize& buttonSize){
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            ContainerButton* currentContainer =
                    new ContainerButton(QString::number(c) + " " + QString::number(r), r, c, ContainerButton::OCCUPIED, grid);
            if(c-2 < r || -c+2 > r)
                currentContainer->setState(ContainerButton::NOTAVAIL);
            if(r < 2)
                currentContainer->setState(ContainerButton::EMPTY);
            connect(currentContainer, &ContainerButton::containerSelected, this, &MainWindow::containerSelected);
            connect(currentContainer, &ContainerButton::containerDeselected, this, &MainWindow::containerDeselected);
            QGridLayout* layout = (QGridLayout*) grid->layout();
            layout->addWidget(currentContainer, r, c);
        }
    }
}

void MainWindow::containerSelected(const ContainerButton* button){
    qDebug() << button->getRow() << " " << button->getCol() << " Selected" << '\n';
}

void MainWindow::containerDeselected(const ContainerButton* button){
    qDebug() << button->getRow() << " " << button->getCol() << " Deselected" << '\n';
}

void MainWindow::on_nextButton_clicked()
{
    current_step += 1;
    if(current_step == steps.size() - 1) {
        ui->nextButton->setDisabled(true);
        ui->doneButton->setEnabled(true);
    }
}
void MainWindow::Login(bool auth){
    ui->actionImport_Manifest->setEnabled(auth);
    ui->actionLogout->setEnabled(auth);
    ui->actionCreate_annotation->setEnabled(auth);
    ui->actionLogin->setEnabled(!auth);
}

void MainWindow::acceptLoginDialog(const QString& username, const QString& password){
    Login(username=="Admin" && password =="Password");
}

void MainWindow::acceptAnnotateDialog(const QString& annotation){
    qDebug() << annotation;
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
        connect(loggin,
                &Dialog::Login,
                this,
                &MainWindow::acceptLoginDialog);
    }
    loggin->exec();

}


void MainWindow::on_actionLogout_triggered()
{
    Login(false);
}


void MainWindow::on_actionCreate_annotation_triggered()
{
    if(annotation == nullptr){
        annotation = new Annotation(this);
        annotation->setModal(true);
        connect(annotation,
                &Annotation::acceptAnnotation,
                this,
                &MainWindow::acceptAnnotateDialog);
    }
    annotation->exec();

}

