#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QMainWindow>
#include"cclient.h"
#include"waitdlg.h"
#include"qjsoncmd.h"
#include"areaview.h"
namespace Ui {
class MainView;
}

class MainView : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainView(QWidget *parent = nullptr);
    ~MainView();
signals:
    void loginResult(int);
    void regResult(const QJsonObject& );
private:
    void doConnect();
    void doLogin();
private slots:
    void sendData(const QJsonObject& json);
    void onNestMsg(netmsg_DataHeader* header);
    void onJsonCMD( const JsonMsg& jsMsg);
    void on_lineEdit_returnPressed();

private:
    Ui::MainView *ui;
    CClient m_Socket;
    waitDlg m_waitDlg;
    AreaView* m_map;
    bool m_bLogined;

};

#endif // MAINVIEW_H
