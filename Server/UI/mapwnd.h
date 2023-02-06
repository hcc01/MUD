#ifndef MAPWND_H
#define MAPWND_H

#include <QLabel>
#include<QMenu>
#include<QAction>
#include"../Object/Area.h"
class MapWnd : public QLabel
{
    Q_OBJECT
public:
    explicit MapWnd(QWidget *parent = nullptr);
    ~MapWnd();
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void contextMenuEvent(QContextMenuEvent*e);

    void setSize(int w,int l,const QString& areaName);
    void resetWinSize();
    //void setArea(Area* area){m_area=area;}
    //void getSize(int& w,int& l){ w=m_width; l=m_length;}
    bool getRoomPos(QPoint &roomPos, const QPoint &mousePos);//获取鼠标位置的房间
    bool save(const QString& fileName);
    bool load(const QString& fileName);
signals:
    void sendPos(const QPoint &pos);
private:
    Area* m_area;
    QPoint m_nowPos;
    QPoint m_selectStart;
    QPoint m_selectEnd;
    int m_unitWidth;//字符宽
    QMenu* m_popMenu;
    QAction* m_editRoom;


};

#endif // MAPWND_H
