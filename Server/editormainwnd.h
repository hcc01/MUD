#ifndef EDITORMAINWND_H
#define EDITORMAINWND_H

#include <QWidget>

#include"./ui/areaeditor.h"
#include"./ui/RoomEditor.h"
namespace Ui {
class EditorMainWnd;
}

class EditorMainWnd : public QWidget
{
    Q_OBJECT

public:
    explicit EditorMainWnd(QWidget *parent = nullptr);
    ~EditorMainWnd();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::EditorMainWnd *ui;
    AreaEditor m_areaEditor;
    RoomEditor m_roomEditor;
};

#endif // EDITORMAINWND_H
