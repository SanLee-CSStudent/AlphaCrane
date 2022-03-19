#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QStringListModel>

#include "containerbutton.h"
#include "annotation.h"
#include "loadcontainer.h"
#include "dialog.h"
#include "Parser.h"
#include <set>

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

struct Position {
    int col;
    int row;
};
struct MoveInfo;

struct PositionComp {
    bool operator() (const Position& p1, const Position& p2) const
    {
        return p1.col < p2.col || p1.row < p2.row;
    }
};


class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  bool buttonFromShip(const ContainerButton *button) ;

  void DisplayNextMove(MoveInfo move) const;

  void setStep(int newStep) const;

  void MoveContainer(MoveInfo move) const;

  ContainerButton* GetContainerButton (int col, int row) const;
private slots:
  void on_nextButton_clicked();

  void on_actionImport_Manifest_triggered();

  void on_doneButton_clicked();

  void on_actionLogin_triggered();

  void acceptLoginDialog(const QString&, const QString&);

  void acceptAnnotateDialog(const QString&);

  void acceptLoadContainer(const QString&, const QString&);

  void on_actionLogout_triggered();

  void on_actionCreate_annotation_triggered();

  void containerSelected(const ContainerButton* button);

  void containerDeselected(const ContainerButton* button);

  void on_pushButton_clicked();

  void on_actionLoad_container_triggered();

private:
  void Login(bool auth);
  void makeContainerGrid(QWidget* grid, int row, int col, const QSize& buttonSize);

  Ui::MainWindow *ui;
  Dialog* loggin = nullptr;
  LoadContainer* loadcontainer = nullptr;
  Annotation* annotation = nullptr;
  QString username;
  int current_step = 0;
  QStringListModel* model;
  Parser* parser;

  std::set<Position, PositionComp> unloadContainer;
};
#endif // MAINWINDOW_H
