#ifndef AREAINFOEDITOR_H
#define AREAINFOEDITOR_H

#include <QDialog>

#include"../Object/Area.h"
namespace Ui {
class AreaInfoEditor;
}

class AreaInfoEditor : public QDialog
{
    Q_OBJECT

public:
    explicit AreaInfoEditor(Area* area, const QPoint& start,const QPoint& end, QWidget *parent = nullptr);
    ~AreaInfoEditor();

private slots:
    void on_buttonBox_accepted();

    void on_pushButtonRoom_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
private:
    void loadRoomInfo();
private:
    Ui::AreaInfoEditor *ui;
    Area* m_area;
    RoomInfo* m_rmInfo;
    QPoint m_startPos;//需要设置的区域起点
    QPoint m_endPos;
};

#endif // AREAINFOEDITOR_H
