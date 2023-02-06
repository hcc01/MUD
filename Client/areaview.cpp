#include "areaview.h"
#include<QPainter>

AreaView::AreaView(QWidget* widget):
    QLabel(widget),
    m_area(new Area),
    m_sight(1),
    m_unitWidth(16),
    m_showRadius(5)
{
    m_pen.setColor(Qt::black);
    m_brush.setColor(Qt::black);
    setFixedSize(m_unitWidth*m_showRadius*2+m_unitWidth,m_unitWidth*m_showRadius*2+m_unitWidth);
    QPalette palette;
    //设置主窗口背景颜色
    palette.setColor(QPalette::Window,QColor(0, 0, 0));
    this->setPalette(palette);
}

AreaView::~AreaView()
{
    delete m_area;
}

void AreaView::init(const QString& areaName, const QPoint &pos, int sight)
{
    m_area->Load(QString("./area/%1.a").arg(areaName));
    m_myPos=pos;
    m_sight=sight;
    update();
}

void AreaView::paintEvent(QPaintEvent *)
{
    if(!m_area) return;
    QPainter painter(this);
    painter.setBackgroundMode(Qt::OpaqueMode);
    painter.setBackground(Qt::gray);
    QFont font;
       // font.setFamily("Microsoft YaHei");
        font.setPointSize((m_unitWidth+4)/2);
        painter.setFont(font);
    RoomInfo* rm;
    int length=m_area->getLength();
    int width=m_area->getWidth();
    int x1,y1,x2,y2;
    int i, j;
    int startX,endX,startY,endY;
    startX=(m_myPos.x()-m_showRadius);
    if(startX<1) startX=1;
    startY=m_myPos.y()-m_showRadius;
    if(startY<1) startY=1;
    endX=m_myPos.x()+m_showRadius;
    if(endX>length) endX=length;
    endY=m_myPos.y()+m_showRadius;
    if(endY>width) endY=width;
    int fixedX=startX-1;
    int fixedY=startY-1;
    endX-=fixedX;
    endY-=fixedY;
    if(endY>width) endY=width;
    for (i =1 ; i <= endX; i++){
        x1 = (i-1) * m_unitWidth;
        x2 = i+fixedX;
        for (j = 1; j <= endY; j++){
            y1 = (j-1) * m_unitWidth;
            y2 = j +fixedY;
            if(!inSight(x2,y2)){
                QBrush brush(Qt::black);
                painter.setBrush(brush);
                painter.drawRect(x1, y1,m_unitWidth,m_unitWidth);
            }
            else{
                rm=m_area->getRoomInfo(x2,y2);
                if(!rm->m_centerFlag.isEmpty()) painter.drawText(QRect(x1, y1,m_unitWidth,m_unitWidth),rm->m_centerFlag);
                else painter.drawText(QRect(x1, y1,m_unitWidth,m_unitWidth), "    ");
            }
        }
    }
    x1=(m_myPos.x()-fixedX-1)* m_unitWidth;
    y1=(m_myPos.y()-fixedY-1)* m_unitWidth;
    painter.drawText(QRect(x1, y1,m_unitWidth,m_unitWidth),Qt::AlignCenter,"🧍‍");
}

void AreaView::mousePressEvent(QMouseEvent *ev)
{

}

void AreaView::showView(const QPoint &myPos)
{
    m_myPos=myPos;
    update();
    if(this->isHidden()) this->show();
}

bool AreaView::inSight(int x, int y)
{
    return x>=(m_myPos.x()-m_sight) && x<=(m_myPos.x()+m_sight)&&y>=(m_myPos.y()-m_sight)&&y<=(m_myPos.y()+m_sight);
}
