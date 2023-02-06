#include "editormainwnd.h"
#include "ui_editormainwnd.h"
EditorMainWnd::EditorMainWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditorMainWnd)
{
    ui->setupUi(this);
    QString s="xi@1,2|";
    QStringList ss;
    s=ss.join("|");
    qDebug()<<s;
}

EditorMainWnd::~EditorMainWnd()
{
    delete ui;
}

void EditorMainWnd::on_pushButton_clicked()
{
    m_areaEditor.show();
}


void EditorMainWnd::on_pushButton_2_clicked()
{
    m_roomEditor.show();
}

