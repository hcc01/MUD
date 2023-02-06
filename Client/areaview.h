#ifndef AREAVIEW_H
#define AREAVIEW_H

#include <QLabel>
#include"../Server/Object/Area.h"
#include<QPen>
class AreaView:public QLabel
{
public:
    AreaView(QWidget *widget=nullptr);
    ~AreaView();
    void init(const QString &areaName, const QPoint& pos, int sight);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *ev);
    void showView(const QPoint& myPos);
    bool inSight(int x,int y);
private:
    int m_sight;
    Area* m_area;
    QPoint m_myPos;
    int m_unitWidth;
    int m_showRadius;
    QPen m_pen;
    QBrush m_brush;

};

#endif // AREAVIEW_H
