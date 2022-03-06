#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileInfo>
#include <sstream>
#define ROWS 8
#define COL 12

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  this->setFixedSize(QSize(1200, 800));
  const QSize btnSize = QSize(65, 65);
  QGridLayout *gridLayout = ui->gridLayout;
  for(int i = 0; i < ROWS; i++) {
      for(int j = 0; j < COL; j++) {
          QPushButton *button = new QPushButton();
          button->setFixedSize(btnSize);
          std::stringstream ss;
          button->setText(QString::number(i) + "," + QString::number(j));
          gridLayout->addWidget(button, i, j);
      }
  }
  gridLayout->setSpacing(2);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_pushButton_2_clicked()
{
    current_step += 1;
    if(current_step == steps.size() - 1) {
        ui->pushButton_2->setDisabled(true);
        ui->pushButton->setEnabled(true);
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


void MainWindow::on_pushButton_clicked()
{
    QMessageBox::information(this, "REMINDER", "Please send manifest to ship!");
}

