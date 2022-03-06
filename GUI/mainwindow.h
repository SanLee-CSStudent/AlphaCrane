#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <vector>


QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
private slots:
  void on_pushButton_2_clicked();

  void on_actionImport_Manifest_triggered();

  void on_pushButton_clicked();

private:
  Ui::MainWindow *ui;
  int current_step = 0;
  std::vector<QString> steps = {"Step 1: yum", "Step 2: yay"};
};
#endif // MAINWINDOW_H
