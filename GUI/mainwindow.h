#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <vector>

#include "containerbutton.h"
#include "annotation.h"
#include "dialog.h"

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
  void on_nextButton_clicked();

  void on_actionImport_Manifest_triggered();

  void on_doneButton_clicked();

  void on_actionLogin_triggered();

  void acceptLoginDialog(const QString&, const QString&);

  void acceptAnnotateDialog(const QString&);

  void on_actionLogout_triggered();

  void on_actionCreate_annotation_triggered();

  void containerSelected(const ContainerButton* button);

  void containerDeselected(const ContainerButton* button);

private:
  void Login(bool auth);
  void makeContainerGrid(QWidget* grid, int row, int col, const QSize& buttonSize);
  Ui::MainWindow *ui;
  Dialog* loggin = nullptr;
  Annotation* annotation = nullptr;
  QString username;
  int current_step = 0;
  std::vector<QString> steps = {"Step 1: yum", "Step 2: yay"};
};
#endif // MAINWINDOW_H
