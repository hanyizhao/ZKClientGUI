#pragma once

#include <QAbstractItemModel>
#include "ZKNodeTreeItem.h"

class ZKNodeTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    ZKNodeTreeModel(QObject* parent);
    ~ZKNodeTreeModel();

    Q_INVOKABLE QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    Q_INVOKABLE QModelIndex parent(const QModelIndex& child) const override;
    Q_INVOKABLE int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    Q_INVOKABLE int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    Q_INVOKABLE QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    // 更新子树的部分数据
    void OnUpdateSubTreeSubValue(std::shared_ptr<ZKNodeTreeItem> pTreeItem);

    ZKNodeTreeItem m_xRootItem; // 根节点

    ZKNodeTreeItem* FindTreeItem(const std::string& strPath, bool bCreateNew);

    // 这个方法谨慎使用
    QModelIndex CarefullyCreateIndex(ZKNodeTreeItem* pNowItem);

private:
    ZKNodeTreeItem* DoFindTreeItem(ZKNodeTreeItem* pNowItem, const std::string& strLeftPath, bool bCreateNew);

    void DoUpdateSubTreeSubValue(ZKNodeTreeItem* pTreeItem, ZKNodeTreeItem* pNewValue);

};
