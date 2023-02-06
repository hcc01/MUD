#include "waitdlg.h"
#include "ui_waitdlg.h"

waitDlg::waitDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::waitDlg)
{
    ui->setupUi(this);
}

waitDlg::~waitDlg()
{
    delete ui;
}
