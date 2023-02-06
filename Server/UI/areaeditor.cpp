#include "areaeditor.h"
#include "ui_areaeditor.h"
#include<QDialog>
#include<QDialogButtonBox>
#include<QLineEdit>
#include<QValidator>
#include<QMessageBox>
#include<QScrollArea>
#include<QFileDialog>
AreaEditor::AreaEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AreaEditor)
{
    ui->setupUi(this);
   // ui->widget->setArea(new Area);
    QScrollArea *s = new QScrollArea(this);
    //s->setFixedSize( 220, 220);
    s->setWidget(ui->widget);
    ui->gridLayout->addWidget(s,1,0,2,1);
    connect(ui->widget,&MapWnd::sendPos,this,&AreaEditor::onPosChanged);
}

AreaEditor::~AreaEditor()
{
    delete ui;
}

void AreaEditor::onPosChanged(const QPoint &p)
{
    ui->lable_pos->setText(QString("(%1,%2)").arg(p.x()).arg(p.y()));
}

void AreaEditor::on_pushButton_new_clicked()
{
    QDialog dlg;
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,&dlg);
    QGridLayout* gl=new QGridLayout(&dlg);
    QLabel* layName=new QLabel("文件名：",&dlg);
    QLabel* layLen=new QLabel("区域长：",&dlg);
    QLabel* laywidth=new QLabel("区域宽：",&dlg);
    QLineEdit* lineEditName=new QLineEdit(&dlg);
    QLineEdit* lineEditLen=new QLineEdit(&dlg);
    QLineEdit* lineEditWidth=new QLineEdit(&dlg);
    gl->addWidget(layName,0,0);
    gl->addWidget(lineEditName,1,0);
    gl->addWidget(layLen,0,1);
    gl->addWidget(lineEditLen,1,1);
    gl->addWidget(laywidth,0,2);
    gl->addWidget(lineEditWidth,1,2);
     gl->addWidget(buttonBox,2,2);
     connect(buttonBox, &QDialogButtonBox::accepted, this, [&](){
         QString name;int len,width;
         name=lineEditName->text();
         if(name.isEmpty()){
             QMessageBox::information(this,"error","请输入拟创建的区域名称。");
             return;
         }
         len=lineEditLen->text().toInt();
         if(len<1||len>100){
             QMessageBox::information(this,"error","区域长度必须在1~100之间。");
             return;
         }
         width=lineEditWidth->text().toInt();
         if(width<1||width>100){
             QMessageBox::information(this,"error","区域宽度必须在1~100之间。");
             return;
         }
         ui->widget->setSize(width,len,name);
         dlg.accept();
     });
    connect(buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    dlg.exec();
}


void AreaEditor::on_pushButton_save_clicked()
{
    QString str=QFileDialog::getSaveFileName(NULL,NULL,"./area/","area(*.a)");
    if(str.isEmpty()) return;
    ui->widget->save(str);
}


void AreaEditor::on_pushButton_open_clicked()
{
    QString str=QFileDialog::getOpenFileName(NULL,NULL,"./area/","area(*.a)");
    if(str.isEmpty()) return;
    ui->widget->load(str);
}

