#ifndef ROOMEDITOR_H
#define ROOMEDITOR_H

#include <QWidget>
#include"../Object/Object.h"
namespace Ui {
class RoomEditor;
}

class RoomEditor : public QWidget
{
    Q_OBJECT

public:
    explicit RoomEditor(QWidget *parent = nullptr);
    ~RoomEditor();
private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

private:
private:
    Ui::RoomEditor *ui;
    Object* ob;
};

#endif // ROOMEDITOR_H
