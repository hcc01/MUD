#ifndef LOGINUI_H
#define LOGINUI_H

#include <QDialog>
#include<qjsoncmd.h>
namespace Ui {
class LoginUI;
}

class LoginUI : public QDialog
{
    Q_OBJECT

public:
    explicit LoginUI(QWidget *parent = nullptr);
    ~LoginUI();
    void onLoginResult(int result);
    void reject()override;
signals:
    void login(const QString&,const QString&);
    void reg(const RegMsg& );
    void regResult(const QJsonObject&);
private slots:
    void on_btLogin_clicked();

    void on_btReg_clicked();


private:
    Ui::LoginUI *ui;
};

#endif // LOGINUI_H
