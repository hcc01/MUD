#include "areainfoeditor.h"
#include "ui_areainfoeditor.h"
#include<QFileDialog>
AreaInfoEditor::AreaInfoEditor(Area *area, const QPoint &start, const QPoint &end, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AreaInfoEditor),
    m_area(area),
    m_startPos(start),
    m_endPos(end)
{
    ui->setupUi(this);
    delete(ui->listWidget->takeItem(0));
    m_rmInfo=new RoomInfo;
    if(start==end) {
        loadRoomInfo();//编辑单个房间时，加载房间信息；
        //m_area->getRoomInfo(start.x(),start.y())->reset();//清空原有信息
    }

}

AreaInfoEditor::~AreaInfoEditor()
{
    delete ui;
    delete m_rmInfo;
}

void AreaInfoEditor::on_buttonBox_accepted()
{
    if(!m_area) return;
    int i,j;
    QString centerFlag= ui->lineEditCenterFlag->text();
    RoomInfo* rm;

    for(i=m_startPos.x();i<=m_endPos.x();i++){
        for(j=m_startPos.y();j<=m_endPos.y();j++){
            rm=m_area->getRoomInfo(i,j);
            rm->m_centerFlag=centerFlag;
            if(ui->checkBoxE->isChecked()) m_area->setDirection(i,j,m_area->E);
            if(ui->checkBoxN->isChecked()) m_area->setDirection(i,j,m_area->N);
            if(ui->checkBoxW->isChecked()) m_area->setDirection(i,j,m_area->W);
            if(ui->checkBoxS->isChecked()) m_area->setDirection(i,j,m_area->S);
            if(ui->checkBoxNE->isChecked()) m_area->setDirection(i,j,m_area->NE);
            if(ui->checkBoxNW->isChecked()) m_area->setDirection(i,j,m_area->NW);
            if(ui->checkBoxSE->isChecked()) m_area->setDirection(i,j,m_area->SE);
            if(ui->checkBoxSW->isChecked()) m_area->setDirection(i,j,m_area->SW);
            if(ui->checkBoxRoom->isChecked()) m_area->setRoom(i,j,ui->labelRoom->text());
            QStringList objs;
            for(int i=0;i<ui->listWidget->count();i++){
                objs.append(ui->listWidget->item(i)->text());
            }
            m_area->setObjects(i,j,objs.join("|"));
        }
    }


}


void AreaInfoEditor::on_pushButtonRoom_clicked()
{
    QString str=QFileDialog::getOpenFileName(NULL,NULL,"./room/","room(*.o)");
    if(str.isEmpty()) return;
    str.replace(QDir::currentPath(),".");
    ui->labelRoom->setText(str);
}


void AreaInfoEditor::on_pushButton_clicked()
{
    QString str=QFileDialog::getOpenFileName(NULL,NULL,"./object/","object(*.o)");
    if(str.isEmpty()) return;
    str.replace(QDir::currentPath(),".");
    int amount=ui->spinBoxAmount->value();
    int chance=ui->spinBoxChance->value();
    ui->listWidget->addItem(QString("%1@%2@%3").arg(str).arg(amount).arg(chance));
}


void AreaInfoEditor::on_pushButton_2_clicked()
{
    int row=ui->listWidget->currentRow();
    qDebug()<<row;
    if(row<0) return;
    QListWidgetItem* a=ui->listWidget->takeItem(row);
    delete a;
}

void AreaInfoEditor::loadRoomInfo()
{
    if(!m_area) return;
    int x=m_startPos.x();
    int y=m_startPos.y();
    ui->labelRoom->setText(m_area->getRoom(m_startPos));
    ui->lineEditCenterFlag->setText(m_area->getCenterFlag(x,y));
    int d=m_area->getDirection(x,y);
    if(d&Area::E) ui->checkBoxE->setChecked(true);
    if(d&Area::W) ui->checkBoxW->setChecked(true);
    if(d&Area::S) ui->checkBoxS->setChecked(true);
    if(d&Area::N) ui->checkBoxN->setChecked(true);
    if(d&Area::SE) ui->checkBoxSE->setChecked(true);
    if(d&Area::SW) ui->checkBoxSW->setChecked(true);
    if(d&Area::NE) ui->checkBoxNE->setChecked(true);
    if(d&Area::NW) ui->checkBoxNW->setChecked(true);
    QStringList objs=m_area->getObjects(x,y);
    //ui->listWidget->clear();
    ui->listWidget->addItems(objs);
}

