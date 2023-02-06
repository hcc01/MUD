#include "mainview.h"
#include "ui_mainview.h"
#include<QMessageBox>
#include<QHostInfo>
#include"loginui.h"
#include"qjsoncmd.h"
#include<QFile>
#include"../Server/Object/Object.h"
#include"../Server/Object/Area.h"
MainView::MainView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainView),
    m_bLogined(false)
{
    m_map=new AreaView;
    m_map->setWindowFlag(Qt::WindowStaysOnTopHint);
    ui->setupUi(this);
    connect(&m_Socket,&CClient::onConnectError,this,[&](const char* error){//断线处理函数
        doConnect();
        if(m_bLogined) doLogin();//已经登录过了，断线后重新登录。（否则就是在登录界面等待登录，不要再重走这个步骤）；目前这个设置不完善，待优化
    });
    connect(&m_Socket,&CClient::netMsg,this,&MainView::onNestMsg,Qt::BlockingQueuedConnection);//第五个参数很重要，否则当服务器连续发送消息时不能及时处理，只重复处理到最后一条。
    connect(&m_Socket,&CClient::sendingData,this,[&](){
        m_waitDlg.exec();
    });
    doConnect();
    doLogin();
    m_bLogined=true;
//    netMsg_Init msg;
//    m_Socket.SendData(&msg);
}

MainView::~MainView()
{
    delete ui;
    delete m_map;
}

void MainView::doConnect()
{
    QHostInfo info = QHostInfo::fromName("127.0.0.1");
    if(m_Socket.Connect(info.addresses().first().toString().toUtf8(),5555)==SOCKET_ERROR){
        int r=QMessageBox::warning(nullptr,"","无法连接服务器","重新连接","退出");
        switch (r) {
            case 0:
            doConnect();
            break;
        case 1:
            exit(0);
            break;
        }
    }

}

void MainView::doLogin()
{
    LoginUI loginUI;
    connect(&loginUI,&LoginUI::login,this,[&](const QString& id, const QString& password){
        netmsg_Login msgLogin;
        memcpy(msgLogin.userName,id.toUtf8().data(),32);
        memcpy(msgLogin.PassWord,password.toUtf8().data(),33);
        m_Socket.SendData(&msgLogin);
    });
    connect(&loginUI,&LoginUI::reg,this,[&](const RegMsg& regInfo){        
        m_Socket.SendData(regInfo.msgInfo());
    });
    connect(this,&MainView::loginResult,&loginUI, &LoginUI::onLoginResult);
    connect(this,&MainView::regResult,&loginUI, &LoginUI::regResult);
    loginUI.exec();
}

void MainView::sendData(const QJsonObject &json)
{
    m_Socket.SendData(json);
}

void MainView::onNestMsg(netmsg_DataHeader *header)
{
    if(m_waitDlg.isModal())m_waitDlg.accept();
    switch (header->cmd) {
    case CMD_S2C_HEART:
    {

    }
        break;
    case CMD_LOGIN_RESULT:
    {
        netmsg_LoginR* lr=(netmsg_LoginR*)header;
        emit loginResult(lr->result);

    }
        break;
    case CMD_JSON_CMD:
    {
        JsonMsg jm(CELLReadStream(header).getJsonData());
        onJsonCMD(jm);

    }
        break;
    default:
        break;
    }
}

void MainView::onJsonCMD(const JsonMsg &jsMsg)
{
    int type=jsMsg.type();
    switch (type) {
    case JC_REG_RETURN:
        emit regResult(jsMsg.msgInfo());
        break;
    case JC_NOTICE:
    {
        NoticeMsg ntMsg(jsMsg.msgInfo());
        ui->noticeMsgBrowser->append(ntMsg.msg());
    }
        break;
    case JC_ENVIRONMENT:
    {
        Object* obj=new Object;
        EnvirontmentMsg evMsg(jsMsg.msgInfo());
        qDebug()<<"jsMsg.msgInfo()"<<jsMsg.msgInfo();
        //处理地图信息
        QString areaName;int x,y;
        evMsg.getAreaMsg(areaName,x,y);
        m_map->init(areaName,QPoint(x,y),3);
        m_map->showNormal();
        //处理房间信息
        obj=new Object;
        QString str=evMsg.roomMsg();
        if(!obj->Load(str)){ return;}
        ui->RoomMsgBrowser->append(QString("%1 -%2(%3,%4)").arg(obj->name()).arg(areaName).arg(x).arg(y));
        ui->RoomMsgBrowser->append(obj->getData().value("desc").toString());
        delete obj;
        //处理环境物件信息
        QMap<QString,int>objs=evMsg.getObjectsInfo();
        if(objs.size()){
            foreach (auto name, objs.keys()) {
                ui->RoomMsgBrowser->append(name);
            }
        }
    }
        break;
    default:
        break;
    }
}

void MainView::on_lineEdit_returnPressed()
{
    CommandMsg cmdMsg(ui->lineEdit->text());
    sendData(cmdMsg.msgInfo());
}

