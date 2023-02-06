#include "mapwnd.h"
#include<QPainter>
#include<QMouseEvent>
#include<QMessageBox>
#include"areainfoeditor.h"
MapWnd::MapWnd(QWidget *parent)
    : QLabel{parent}
    , m_unitWidth(16)
{
    setMouseTracking(true);
    setFixedSize((10)*m_unitWidth*2+10,(10)*m_unitWidth*2+10);
    m_area=new Area;
    m_popMenu=new QMenu(this);
    m_editRoom=new QAction(this);
    m_editRoom->setText("编辑房间");
    connect(m_editRoom,&QAction::triggered,this,[&](){
        QPoint start,end;
        if(m_selectEnd.x()){
            start=m_selectStart;
            end=m_selectEnd;
        }
        else{
            start=end=m_nowPos;
        }
        AreaInfoEditor ae(m_area,start,end);
        ae.exec();

    });
}

MapWnd::~MapWnd()
{
    if(m_area) delete m_area;
}

void MapWnd::paintEvent(QPaintEvent *ev)
{
    if(!m_area) return;
    QPainter painter(this);
    QFont font;
       // font.setFamily("Microsoft YaHei");
        font.setPointSize((m_unitWidth+4)/2);
        painter.setFont(font);
    RoomInfo* rm;
    int length=m_area->getLength();
    int width=m_area->getWidth();
    int x1,y1,x2,y2;
        int i, j;
        for (i = 1; i <= length; i++){
            x1 = i * m_unitWidth*2 - m_unitWidth;
            x2 = i * m_unitWidth*2;
            for (j = 1; j <= width; j++){
                y1 = j * m_unitWidth*2 - m_unitWidth;
                y2 = j * m_unitWidth*2;
                rm=m_area->getRoomInfo(i,j);
               // QPen pen(af.color);
               // painter.setPen(pen);
//                if(m_area->getRoom(i,j).isEmpty()){
                    if(rm->m_centerFlag.isEmpty()){
                       painter.drawRect(x1, y1,m_unitWidth,m_unitWidth);
                    //painter.drawText(QRect(x1, y1,m_unitWidth,m_unit                        Width),Qt::AlignCenter,"🌼");
                    }
                    else painter.drawText(QRect(x1, y1,m_unitWidth,m_unitWidth),rm->m_centerFlag);
                    //painter.drawText(QRect(x1, y1,m_unitWidth,m_unitWidth),Qt::AlignCenter,"🌼");
//                }
//               else {
//                    QPen pen(Qt::cyan);
//                    painter.setPen(pen);
//                   painter.drawRect(x1, y1,m_unitWidth,m_unitWidth);
//                   painter.drawText(QRect(x1, y1,m_unitWidth,m_unitWidth),"▲");
//                   painter.setBrush(QBrush());
//               }
               int d;
               if(d=m_area->getDirection(i,j)){
                   if(d&m_area->E){
                       painter.drawLine(x1+m_unitWidth,y1+m_unitWidth/2,x1+m_unitWidth*2,y1+m_unitWidth/2);
                   }
                   if(d&m_area->S){
                       painter.drawLine(x1+m_unitWidth/2,y1+m_unitWidth,x1+m_unitWidth/2,y1+m_unitWidth*2);
                   }
                   if(d&m_area->SE){
                        painter.drawLine(x1+m_unitWidth,y1+m_unitWidth,x1+m_unitWidth*2,y1+m_unitWidth*2);
                   }
                   if(d&m_area->NE){
                        painter.drawLine(x1+m_unitWidth,y1,x1+m_unitWidth*2,y1-m_unitWidth);
                   }
               }
            }
        }
        if(m_selectEnd.x()){
            QPen pen(Qt::red);
            x1=m_selectStart.x()* m_unitWidth*2 - m_unitWidth-5;
            y1=m_selectStart.y()* m_unitWidth*2 - m_unitWidth-5;
            x2=m_selectEnd.x()* m_unitWidth*2+5;
            y2=m_selectEnd.y()* m_unitWidth*2+5;
            painter.setPen(pen);
            painter.drawRect(x1, y1,x2-x1,y2-y1);
            painter.setPen(QPen());
        }
        else if(m_nowPos.x()){
            QPen pen(Qt::red);
            x1=m_nowPos.x()* m_unitWidth*2 - m_unitWidth-5;
            y1=m_nowPos.y()* m_unitWidth*2 - m_unitWidth-5;
            painter.setPen(pen);
            painter.drawRect(x1, y1,m_unitWidth+10,m_unitWidth+10);
            painter.setPen(QPen());

        }

}

void MapWnd::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::RightButton){
//        setMouseTracking(true);
//        m_selectStart=QPoint();
//        m_selectEnd=QPoint();
        return;
    }
    if(!getRoomPos(m_selectStart,ev->pos())) return;
}

void MapWnd::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::RightButton) return;
    if(!m_selectStart.x()) return;
    if(!getRoomPos(m_selectEnd,ev->pos())) return;

    if(m_selectEnd==m_selectStart) {//取消掉多选
        setMouseTracking(true);
        m_selectStart=QPoint();
        m_selectEnd=QPoint();
        update();
        return;
    }
    setMouseTracking(false);
    int x1,x2,y1,y2;
    x1=m_selectStart.x();
    x2=m_selectEnd.x();
    y1=m_selectStart.y();
    y2=m_selectEnd.y();
    if(x1>x2){
        m_selectStart.setX(x2);
        m_selectEnd.setX(x1);
    }
    if(y1>y2){
        m_selectStart.setY(y2);
        m_selectEnd.setY(y1);
    }
    update();
}

void MapWnd::mouseMoveEvent(QMouseEvent *ev)
{
    QPoint pos;
    if(!getRoomPos(pos,ev->pos())) return;
    if(pos==m_nowPos) return;
    m_nowPos=pos;
     emit sendPos(m_nowPos);
    QRect rect=frameGeometry();
   // update(QRect(2-rect.x(),2-rect.y(),200,200));
    update();
}

void MapWnd::contextMenuEvent(QContextMenuEvent *e)
{
    m_popMenu->clear();
    m_popMenu->addAction(m_editRoom);
    m_popMenu->exec(QCursor::pos());
}

void MapWnd::setSize(int w, int l, const QString &areaName)
{
    m_area->setSize(l,w);
    m_area->setAreaName(areaName);
    for(int i=0;i<l;i++){
        for(int j=0;j<w;j++){

            m_area->getRoomInfo(i,j)->reset();
        }
    }
   resetWinSize();
}

void MapWnd::resetWinSize()
{
    setFixedSize((m_area->getWidth())*m_unitWidth*2+10,(m_area->getLength())*m_unitWidth*2+10);
}

bool MapWnd::getRoomPos(QPoint &roomPos, const QPoint &p)
{
    int a,b;
    a = (p.x()) / m_unitWidth + 1;
        if (a % 2) return false;
        b = (p.y()) / m_unitWidth + 1;
        if (b % 2) return false;
        a /= 2; b /= 2;
        if (a > m_area->getLength()) return false;
        if (b > m_area->getWidth()) return false;
        roomPos.setX(a);
        roomPos.setY(b);
        return true;
}

bool MapWnd::save(const QString &fileName)
{
    return m_area->save(fileName);
}

bool MapWnd::load(const QString &fileName)
{
    qDebug()<<m_area->getLength();

    if(!m_area->Load(fileName)){
        QMessageBox::information(nullptr,"error","无法加载文件："+fileName);
        return false;
    }
    resetWinSize();
    qDebug()<<m_area->getLength();
    return true;
}
