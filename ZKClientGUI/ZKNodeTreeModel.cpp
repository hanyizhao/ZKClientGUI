#include "ZKNodeTreeModel.h"
#include <unordered_set>
#include <QDebug>

ZKNodeTreeModel::ZKNodeTreeModel(QObject* parent)
    : QAbstractItemModel(parent), m_xRootItem(nullptr, "", "", QVariant(), -1)
{
    auto pChild = m_xRootItem.AddChild("", QVariant());

    /*for (int i = 0; i < 10; i++)
    {
        auto pSubChild = pChild->AddChild(std::to_string(i) + " XXXXXXXXXXXXXX", QString::fromStdString(std::to_string(i) + " XXXXXXXXXXXXXX"));
        for (int l = 0; l < 10; l++)
        {
            pSubChild->AddChild(std::to_string(i) + "." + std::to_string(l) + " XXXXXXXXXXXXXX", QString::fromStdString(std::to_string(l) + " XXXXXXXXXXXXXX"));
        }
    }*/
}

ZKNodeTreeModel::~ZKNodeTreeModel()
{
}

Q_INVOKABLE QModelIndex ZKNodeTreeModel::index(int row, int column, const QModelIndex& parent /*= QModelIndex()*/) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    const ZKNodeTreeItem* pParentItem = nullptr;

    // 根节点
    if (!parent.isValid())
    {
        pParentItem = &m_xRootItem;
    }
    else // 普通节点
    {
        pParentItem = static_cast<ZKNodeTreeItem*>(parent.internalPointer());
    }

    // 找到子节点
    auto pChildItem = pParentItem->GetChild(row);

    // 子节点不存在
    if (pChildItem == nullptr)
    {
        return QModelIndex();
    }

    return createIndex(row, column, pChildItem);
}

Q_INVOKABLE QModelIndex ZKNodeTreeModel::parent(const QModelIndex& child) const
{
    // 根节点没有父节点
    if (!child.isValid())
    {
        return QModelIndex();
    }

    auto pChildItem = static_cast<ZKNodeTreeItem*>(child.internalPointer());

    auto pParentItem = pChildItem->GetParent();

    if (pParentItem == nullptr || pParentItem == &m_xRootItem)
    {
        return QModelIndex();
    }

    return createIndex(pParentItem->GetSelfRow(), 0, pParentItem);
}

Q_INVOKABLE int ZKNodeTreeModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
    if (parent.column() > 0)
    {
        return 0;
    }

    // 根节点调用
    if (!parent.isValid())
    {
        return m_xRootItem.GetChildCount();
    }

    // 普通节点调用
    return static_cast<ZKNodeTreeItem*>(parent.internalPointer())->GetChildCount();
}

Q_INVOKABLE int ZKNodeTreeModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
    return 1;
}

Q_INVOKABLE QVariant ZKNodeTreeModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    auto* pNowItem = static_cast<ZKNodeTreeItem*>(index.internalPointer());

    return pNowItem->data(index.column());
}

void ZKNodeTreeModel::OnUpdateSubTreeSubValue(std::shared_ptr<ZKNodeTreeItem> pTreeItem)
{
    auto pNowTreeItem = FindTreeItem(pTreeItem->GetFullPath(), true);
    if (pNowTreeItem == nullptr)
    {
        qDebug() << "Failed to Find or create item: " << pTreeItem->GetFullPath().c_str();
        return;
    }

    DoUpdateSubTreeSubValue(pNowTreeItem, pTreeItem.get());

    emit layoutChanged();
}

ZKNodeTreeItem* ZKNodeTreeModel::FindTreeItem(const std::string& strPath, bool bCreateNew)
{
    // 格式错误
    if (strPath.empty() || strPath.at(0) != '/')
    {
        return nullptr;
    }

    return DoFindTreeItem(&m_xRootItem, strPath, true);
}

QModelIndex ZKNodeTreeModel::CarefullyCreateIndex(ZKNodeTreeItem* pNowItem)
{
    if (pNowItem == nullptr)
    {
        return QModelIndex();
    }

    return createIndex(pNowItem->GetSelfRow(), 0, pNowItem);
}

ZKNodeTreeItem* ZKNodeTreeModel::DoFindTreeItem(ZKNodeTreeItem* pNowItem, const std::string& strLeftPath, bool bCreateNew)
{
    std::string strFirstName;
    std::string strLeft;

    auto nFindSlash = strLeftPath.find('/');
    if (nFindSlash == std::string::npos)
    {
        strFirstName = strLeftPath;
        strLeft = "";
    }
    else
    {
        strFirstName = strLeftPath.substr(0, nFindSlash);
        strLeft = strLeftPath.substr(nFindSlash + 1);
    }

    // 节点名除了根节点不能是空
    if (pNowItem->GetPathDepth() != -1 && strFirstName.empty())
    {
        return nullptr;
    }

    auto pChild = pNowItem->GetChildByName(strFirstName);
    if (pChild == nullptr && bCreateNew)
    {
        pChild = pNowItem->AddChild(strFirstName, QVariant());
    }

    // 没有找到
    if (pChild == nullptr)
    {
        return nullptr;
    }

    if (strLeft.empty())
    {
        return pChild;
    }
    else
    {
        return DoFindTreeItem(pChild, strLeft, bCreateNew);
    }
}

void ZKNodeTreeModel::DoUpdateSubTreeSubValue(ZKNodeTreeItem* pTreeItem, ZKNodeTreeItem* pNewValue)
{
    // 如果节点被删除了
    if (pNewValue->m_bNoNode)
    {
        auto pParent = pTreeItem->GetParent();
        if (pParent != nullptr)
        {
            pParent->RemoveChild(pTreeItem->GetNodeName());
        }

        return;
    }

    pTreeItem->SetNodeValue(pNewValue->GetNodeValue());
    pTreeItem->m_xStat = pNewValue->m_xStat;

    std::unordered_set<std::string> setNeedDeletedChild;
    for (int i = 0; i < pTreeItem->GetChildCount(); i++)
    {
        auto pChild = pTreeItem->GetChild(i);
        if (pChild != nullptr)
        {
            setNeedDeletedChild.insert(pChild->GetNodeName());
        }
    }

    for (int i = 0; i < pNewValue->GetChildCount(); i++)
    {
        auto pNewChild = pNewValue->GetChild(i);
        if (pNewChild == nullptr)
        {
            continue;
        }

        if (pNewChild->m_bNoNode)
        {
            pTreeItem->RemoveChild(pNewChild->GetNodeName());
        }
        else
        {
            setNeedDeletedChild.erase(pNewChild->GetNodeName());

            auto pMyChild = pTreeItem->GetChildByName(pNewChild->GetNodeName());
            if (pMyChild == nullptr)
            {
                pMyChild = pTreeItem->AddChild(pNewChild->GetNodeName(), QVariant());
            }

            if (pNewValue->m_bQueryNextLevel)
            {
                DoUpdateSubTreeSubValue(pMyChild, pNewChild);
            }
        }
    }

    // 删除需要删除的节点
    for (auto& strName : setNeedDeletedChild)
    {
        pTreeItem->RemoveChild(strName);
    }
}
