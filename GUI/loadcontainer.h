#ifndef LOADCONTAINER_H
#define LOADCONTAINER_H

#include <QDialog>

namespace Ui {
class LoadContainer;
}

class LoadContainer : public QDialog
{
    Q_OBJECT

public:
    explicit LoadContainer(QWidget *parent = nullptr);
    ~LoadContainer();
signals:
    void Load(const QString&, const QString&);

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::LoadContainer *ui;
};

#endif // LOADCONTAINER_H
