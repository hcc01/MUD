#include "RoomEditor.h"
#include "ui_RoomEditor.h"
#include<QJsonDocument>
#include<QFileDialog>
RoomEditor::RoomEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoomEditor)
{
    ui->setupUi(this);
    ob=new Object;
    ui->comboBox->addItems(DynObjectFactory::ClassNames);
//    for(int i=0;i<DynObjectFactory::ClassNames.count();i++){
//        ui->comboBox->addItem(DynObjectFactory::ClassNames.at(i));
//    }
}

RoomEditor::~RoomEditor()
{
    delete ui;
    delete ob;
}

void RoomEditor::on_pushButton_4_clicked()
{
    ob->setClass(ui->comboBox->currentText());
    ui->textBrowser->clear();
    ui->textBrowser->append(QString("房间类：%1").arg(ui->comboBox->currentText()));
    if(!ui->lineEditName->text().isEmpty()) ob->set("name",ui->lineEditName->text());
    if(!ui->lineEditDesc->text().isEmpty()) ob->set("desc",ui->lineEditDesc->text());
    if(!ui->lineEditKey->text().isEmpty()&&!ui->lineEdit_4->text().isEmpty()) ob->set(ui->lineEditKey->text(),ui->lineEdit_4->text());
    ui->textBrowser->append(QString(QJsonDocument(ob->getData()).toJson()));
}


void RoomEditor::on_pushButton_3_clicked()
{
    QString str=QFileDialog::getSaveFileName(NULL,NULL,"./room/","room(*.o)");
    if(str.isEmpty()) return;
    ob->Save(str);
}


void RoomEditor::on_pushButton_clicked()
{

    QString str=QFileDialog::getOpenFileName(NULL,NULL,"./room/","room(*.o)");
    if(str.isEmpty()) return;
    this->setWindowTitle(str);
    ob->Load(str);
    ui->textBrowser->clear();
    ui->textBrowser->append(QString("房间类：%1").arg(ob->className()));
    ui->textBrowser->append(QString(QJsonDocument(ob->getData()).toJson()));
}

