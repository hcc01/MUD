#include "filetree.h"

FileTree::FileTree(QWidget *parent, QString startPath, QString filter)
    : QTreeView{parent}
    , m_startPath(startPath)
    , m_filter(filter)
{
    m_model=new QFileSystemModel(this);
    m_model->setRootPath(m_startPath);
    setModel(m_model);
    setRootIndex(m_model->index("./"));
}
