#ifndef REGDIALOG_H
#define REGDIALOG_H

#include <QDialog>
#include"qjsoncmd.h"
#include<QJsonObject>
namespace Ui {
class RegDialog;
}

class RegDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegDialog(QWidget *parent = nullptr);
    ~RegDialog();
signals:
    void reg(const RegMsg&);
public slots:
    void on_pushButton_clicked();
    void onRegResult(const QJsonObject&);
private:
    Ui::RegDialog *ui;
};

#endif // REGDIALOG_H
