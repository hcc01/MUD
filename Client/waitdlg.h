#ifndef WAITDLG_H
#define WAITDLG_H

#include <QDialog>

namespace Ui {
class waitDlg;
}

class waitDlg : public QDialog
{
    Q_OBJECT

public:
    explicit waitDlg(QWidget *parent = nullptr);
    ~waitDlg();

private:
    Ui::waitDlg *ui;
};

#endif // WAITDLG_H
