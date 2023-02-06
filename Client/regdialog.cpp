#include "regdialog.h"
#include "ui_regdialog.h"
#include<QMessageBox>
#include<QCryptographicHash>
RegDialog::RegDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegDialog)
{
    ui->setupUi(this);   
}

RegDialog::~RegDialog()
{
    delete ui;
}

void RegDialog::on_pushButton_clicked()
{
    if(ui->lineEditID->text().isEmpty()){
        QMessageBox::information(this,"erroe","请输入账号ID");
        return;
    }
    if(ui->lineEdit_PassWD1->text().isEmpty()){
        QMessageBox::information(this,"erroe","请输入账号密码");
        return;
    }
    if(ui->lineEdit_Name->text().isEmpty()){
        QMessageBox::information(this,"erroe","请输入角色大名");
        return;
    }
    if(ui->lineEdit_PassWD1->text()!=ui->lineEdit_PassWD2->text()){
        QMessageBox::information(this,"erroe","两次密码不一致，请确认");
        return;
    }
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(ui->lineEdit_PassWD1->text().toLocal8Bit());
    emit reg(RegMsg(ui->lineEditID->text(),QString(hash.result().toHex()),ui->lineEdit_Name->text(),ui->comboBox->currentIndex()));
}

void RegDialog::onRegResult(const QJsonObject &msgInfo)
{
    RegResultMsg rrMsg(msgInfo);
    if(!rrMsg.isSuccess()){
        QMessageBox::information(this,"注册失败",rrMsg.msg());
        return;
    }
    QMessageBox::information(this,"注册成功","注册成功");
    accept();
}

