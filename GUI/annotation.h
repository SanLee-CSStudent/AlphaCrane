#ifndef ANNOTATION_H
#define ANNOTATION_H

#include <QDialog>

namespace Ui {
class Annotation;
}

class Annotation : public QDialog
{
    Q_OBJECT

public:
    explicit Annotation(QWidget *parent = nullptr);
    ~Annotation();
signals:
    void acceptAnnotation(const QString& annotate);
private slots:
    void on_pushButton_clicked();

private:
    Ui::Annotation *ui;
};

#endif // ANNOTATION_H
