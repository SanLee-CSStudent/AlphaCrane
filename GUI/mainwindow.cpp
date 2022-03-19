#include <QFileInfo>
#include <QFile>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QObject>
#include <QStringList>
#include <QDate>

#include "containerbutton.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "dialog.h"

#include "Balance.h"
#include "Solver.h"

#define SHIP_ROWS 8
#define SHIP_COLS 12
#define BUFF_ROWS 4
#define BUFF_COLS 24

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), model(new QStringListModel(this)){
      parser = new Parser();
      ui->setupUi(this);
      this->setFixedSize(1500, 900);
      makeContainerGrid(ui->shipWidget, SHIP_ROWS, SHIP_COLS, QSize(82,82));
      makeContainerGrid(ui->bufferWidget, BUFF_ROWS, BUFF_COLS, QSize(40, 40));
      on_actionLogin_triggered();
      ui->listView->setModel(model);
      setStep(0);
}
bool operator==(Position p1, Position p2){
    return p1.col == p2.col && p2.row == p1.row;
}
QString getDate(){
    return QDateTime::currentDateTime().toString("MM DD YYYY: h:m:s ");
}

struct MoveInfo{
    QString name;
    Position from;
    Position to;
    static QRegularExpression expression ;

    static MoveInfo Parse(const QString& step_string){
        QRegularExpressionMatch matches = expression.match(step_string);
        MoveInfo move;
        move.name = matches.captured(1);
        move.from = { matches.captured(2).toInt(), matches.captured(3).toInt() };
        move.to = { matches.captured(4).toInt(), matches.captured(5).toInt() };

        return move;
    }
};

QRegularExpression MoveInfo::expression = QRegularExpression("(\\w+): (\\d),(\\d) -> (\\d),(\\d)");

MainWindow::~MainWindow() { delete ui; }

void MainWindow::makeContainerGrid(QWidget *grid, int rows, int cols, const QSize& buttonSize){
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            ContainerButton* currentContainer =
                    new ContainerButton(QString::number(r) + " " + QString::number(c), r, c, ContainerButton::NOTAVAIL, grid);
            connect(currentContainer, &ContainerButton::containerSelected, this, &MainWindow::containerSelected);
            connect(currentContainer, &ContainerButton::containerDeselected, this, &MainWindow::containerDeselected);
            QGridLayout* layout = (QGridLayout*) grid->layout();
            layout->addWidget(currentContainer, r, c);
        }
    }
}
bool MainWindow::buttonFromShip(const ContainerButton* button){
    return button->parentWidget() == ui->shipWidget;
}
void MainWindow::containerSelected(const ContainerButton* button){
    Position p = { button->getRow(), button->getCol()};
    unloadContainer.insert(p);
}

void MainWindow::containerDeselected(const ContainerButton* button){
    Position p = { button->getRow(), button->getCol()};
    unloadContainer.erase(p);
}
void MainWindow::setStep(int next_step = 0) const{
    ui->listView->selectionModel()->clear();
    QModelIndex currentIndex = model->index(next_step, 0);
    ui->listView->selectionModel()->select(currentIndex, QItemSelectionModel::Select);
}

void MainWindow::MoveContainer(MoveInfo move) const{
    stringstream ss;

    if (move.to == move.from){
        ContainerButton* containerButton = GetContainerButton(move.from.col, move.from.row);
        ss << move.from.row << " " << move.from.col;
        containerButton->setText(QString::fromStdString(ss.str()));
        containerButton->setState(ContainerButton::EMPTY);
        return;
    }
    if (move.from.col == 0 && move.from.row == 0){
        ContainerButton* containerButton = GetContainerButton(move.to.col, move.to.row);
        ss << move.to.row << " " << move.to.col;
        containerButton->setText(move.name);
        containerButton->setState(ContainerButton::OCCUPIED);
        return;
    }
    ContainerButton* containerButton = GetContainerButton(move.from.col, move.from.row);
    ContainerButton* newButton = GetContainerButton(move.to.col, move.to.row);
    ss << move.from.col << " " << move.from.row;
    containerButton->setText(QString::fromStdString(ss.str()));
    containerButton->setState(ContainerButton::EMPTY);

    newButton->setText(move.name);
    newButton->setState(ContainerButton::OCCUPIED);
}
void MainWindow::on_nextButton_clicked()
{
    if(model->stringList().empty()){
        return;
    }
    MoveInfo move = MoveInfo::Parse(model->stringList().at(current_step));
    MoveContainer(move);
    qDebug() << getDate() << model->stringList().at(current_step);
    current_step++;
    Container prev = parser->getParseGrid()->getContainer(move.from.col, move.from.row);

    if (move.to == move.from ){
        qDebug() << getDate() << move.name << " is " << "offloaded";
        parser->getParseGrid()->emptyContainer(move.from.col, move.from.row);
        unloadContainer.erase(move.from);
    }else if(move.from.col == 0 && move.from.row == 0){
        qDebug() << getDate() << move.name << " is " << "onloaded";
        Container c = {0, move.name.toStdString(), {char(move.to.col), char(move.to.row)}};
        parser->getParseGrid()->addContainer(move.to.col, move.to.row, c);
        parser->getParseGrid()->print();
    }else{
        parser->getParseGrid()->moveContainer(prev, move.to.col, move.to.row);
    }
    if(current_step == model->stringList().size()) {
        unloadContainer.clear();
        ui->nextButton->setDisabled(true);
        ui->doneButton->setEnabled(true);
        return;
    }
    move = MoveInfo::Parse(model->stringList().at(current_step));
    setStep(current_step);
    DisplayNextMove(move);

}
void MainWindow::DisplayNextMove(MoveInfo move) const{
    ContainerButton* nextPosition = GetContainerButton(move.to.col, move.to.row);
    nextPosition->setState(ContainerButton::MOVING);
}
void MainWindow::Login(bool auth){
    ui->actionImport_Manifest->setEnabled(auth);
    ui->actionLogout->setEnabled(auth);
    ui->actionCreate_annotation->setEnabled(auth);
    ui->actionLogin->setEnabled(!auth);
}

void MainWindow::acceptLoginDialog(const QString& username, const QString& password){
    if(username == "Admin")
        qDebug() << getDate() << username << " signs in";
    Login(true);
}

void MainWindow::acceptAnnotateDialog(const QString& annotation){
    qDebug() << getDate() << username << ": " << annotation;
}

void MainWindow::acceptLoadContainer(const QString & name, const QString & weight)
{
    Container c = {weight.toInt(), name.toStdString() , {0, 0}};
    ui->doneButton->setDisabled(true);
    ui->nextButton->setEnabled(true);
    vector<Container> load;
    load.push_back(c);
    vector<string> solution = Load(load, parser->getParseGrid());
    if(solution.empty()) {
        qDebug() << "no solution";
        return;
    }
    QStringList list;
    for(string s : solution){
        QString currentString = QString::fromStdString(s);
        list << currentString;
    }
    model->setStringList(list);
    current_step = 0;
    MoveInfo move = MoveInfo::Parse(model->stringList().at(current_step));
    setStep(current_step);
    DisplayNextMove(move);
    ui->pushButton->setDisabled(true);
}

ContainerButton* MainWindow::GetContainerButton(int column, int row) const{
    QGridLayout* layout = qobject_cast<QGridLayout*>(ui->shipWidget->layout());
    return qobject_cast<ContainerButton*>(layout->itemAtPosition(column, row)->widget());
}

void MainWindow::on_actionImport_Manifest_triggered()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Import the manifest");
    if(file_name == "")
        return;

    QFile file(file_name);
    QFileInfo file_info(file);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text ))
    {
        qDebug() << getDate() << "Can't open manifest file";
    }
    parser->parse(file_name.toStdString());
    ContainerGrid* containerGrid = parser->getParseGrid();
    qDebug() << getDate() << "Manifest " << file_name << "opened";

    for(uint32_t r = 0; r < containerGrid->getRow(); ++r){
        for(uint32_t c = 0; c < containerGrid->getColumn(); ++c)
        {
            Container currentContainer = containerGrid->getContainer(c, r);
            ContainerButton* currentContainerButton = GetContainerButton(c, r);
            std::string name = currentContainer.name;
            QString newText = QString::number(r) + " " + QString::number(c);
            if(name == "UNUSED"){
                currentContainerButton->setState(ContainerButton::EMPTY);
            }else if(name == "NAN"){
                currentContainerButton->setState(ContainerButton::NOTAVAIL);
            }else{
                newText = QString::fromStdString(name);
                currentContainerButton->setState(ContainerButton::OCCUPIED);
            }
            currentContainerButton->setText(newText);
        }
    }
    model->setStringList(QStringList());
    setStep(0);
    ui->label->setText(file_info.baseName());
}

void MainWindow::on_doneButton_clicked()
{
    ui->pushButton->setEnabled(true);
    ui->nextButton->setEnabled(true);
    ui->actionBalance->setEnabled(true);
    ui->doneButton->setDisabled(true);
    std::string outfile = "/home/thuanvu/" + ui->label->text().toStdString() + "_OUTBOUND.txt";

    qDebug() << getDate() << "Finished a Cycle. Manifest " << QString::fromStdString(outfile) << "written to desktop" <<
                "and a reminder pop-up to operator to send file was displayed.";

    parser->createManifest(outfile);
    model->setStringList(QStringList());
    QMessageBox::information(this, "REMINDER", QString::fromStdString("Please send manifest to ship!\n" + outfile));
}

void MainWindow::on_actionLogout_triggered()
{
    Login(false);
    qDebug() << getDate() << loggin->username << " signs out";
}

void MainWindow::on_pushButton_clicked()
{
    QStringList list;
    if (unloadContainer.empty()){
        QMessageBox::information(this, "NO SELECTION", "No selection made yet!");
        return;
    }
    vector<Container> unload;
    for(Position p : unloadContainer){
        Container c = parser->getParseGrid()->getContainer(p.col, p.row);
        unload.push_back(c);
    }
    vector<string> solution = Unload(unload, parser->getParseGrid());
    if(solution.empty()) {
        qDebug() << "[ERROR]" << getDate()<< "No solution found.";
        return;
    }
    for(string s : solution){
        QString currentString = QString::fromStdString(s);
        list << currentString;
    }
    model->setStringList(list);
    current_step = 0;
    MoveInfo move = MoveInfo::Parse(model->stringList().at(current_step));
    qDebug() << move.name << move.to.col << move.to.row;
    setStep(current_step);
    DisplayNextMove(move);
    ui->pushButton->setDisabled(true);
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

void MainWindow::on_actionLoad_container_triggered()
{
    if(loadcontainer == nullptr){
        loadcontainer = new LoadContainer();
        loadcontainer->setModal(true);
        connect(loadcontainer,
                &LoadContainer::Load,
                this,
                &MainWindow::acceptLoadContainer);
    }
    loadcontainer->exec();
}


void MainWindow::on_actionBalance_triggered()
{
    QStringList list;

    vector<Container> unload;

    vector<string> solution = Solve(parser->getParseGrid());

    if(solution.empty()){
        qDebug() << "no solution";
        return;
    }

    for(string s : solution){
        QString currentString = QString::fromStdString(s);
        qDebug() << currentString;
        list << currentString;
    }
    model->setStringList(list);
    current_step = 0;
    MoveInfo move = MoveInfo::Parse(model->stringList().at(current_step));
    setStep(current_step);
    DisplayNextMove(move);
    ui->actionBalance->setDisabled(true);
}

