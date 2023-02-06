#ifndef FILETREE_H
#define FILETREE_H

#include <QTreeView>
#include<QModelIndex>
#include<QFileSystemModel>
#include<QFileInfo>
class FileTree : public QTreeView
{
    Q_OBJECT
public:
    explicit FileTree(QWidget *parent = nullptr,QString startPath=QDir::currentPath(), QString filter="");
signals:


private:
    QFileSystemModel* m_model;
    QString m_startPath;
    QString m_filter;
};

#endif // FILETREE_H
