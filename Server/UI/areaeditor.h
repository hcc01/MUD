#ifndef AREAEDITOR_H
#define AREAEDITOR_H

#include <QWidget>

namespace Ui {
class AreaEditor;
}

class AreaEditor : public QWidget
{
    Q_OBJECT

public:
    explicit AreaEditor(QWidget *parent = nullptr);
    ~AreaEditor();

    void onPosChanged(const QPoint&);
private slots:
    void on_pushButton_new_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_open_clicked();

private:
    Ui::AreaEditor *ui;
};

#endif // AREAEDITOR_H
