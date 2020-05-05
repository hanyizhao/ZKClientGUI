#include "ZKNodeTreeItem.h"
#include <QString>

ZKNodeTreeItem::ZKNodeTreeItem(ZKNodeTreeItem* pParentItem, const std::string& strParentPath, const std::string& strNodeName,
                               const QVariant& strNodeValue, int nPathDepth)
    : m_pParentItem(pParentItem), m_strNodeName(strNodeName), m_strNodeValue(strNodeValue), m_nPathDepth(nPathDepth),
      m_strNodeFullPath(nPathDepth < 0 ? "" : ((nPathDepth == 1 ? "" : strParentPath) + "/" + strNodeName))
{
    m_xStat = { 0 };
}

ZKNodeTreeItem::~ZKNodeTreeItem()
{
    // 数组中的指针依次delete
    qDeleteAll(m_vecChildItems);
}

void ZKNodeTreeItem::SetParent(ZKNodeTreeItem* pParentItem)
{
    m_pParentItem = pParentItem;
}

ZKNodeTreeItem* ZKNodeTreeItem::AddChild(const std::string& strNodeName, const QVariant& strNodeValue)
{
    // 如果发现重复，则取消添加子节点
    for (auto pItem : m_vecChildItems)
    {
        if (pItem != nullptr && pItem->m_strNodeName == strNodeName)
        {
            return nullptr;
        }
    }

    ZKNodeTreeItem* pChildItem = new ZKNodeTreeItem(this, m_strNodeFullPath, strNodeName, strNodeValue, m_nPathDepth + 1);
    m_vecChildItems.push_back(pChildItem);

    return pChildItem;
}

ZKNodeTreeItem* ZKNodeTreeItem::GetChild(const int nRow) const
{
    if (nRow >= 0 && nRow < m_vecChildItems.size())
    {
        return m_vecChildItems[nRow];
    }

    return nullptr;
}

ZKNodeTreeItem* ZKNodeTreeItem::GetChildByName(const std::string& strName) const
{
    for (auto pItem : m_vecChildItems)
    {
        if (pItem != nullptr && pItem->m_strNodeName == strName)
        {
            return pItem;
        }
    }

    return nullptr;
}

void ZKNodeTreeItem::RemoveChild(const std::string& strName)
{
    for (auto it = m_vecChildItems.begin(); it != m_vecChildItems.end(); it++)
    {
        if (*it != nullptr && (*it)->m_strNodeName == strName)
        {
            m_vecChildItems.erase(it);
            return;
        }
    }
}

int ZKNodeTreeItem::GetChildCount() const
{
    return m_vecChildItems.count();
}

int ZKNodeTreeItem::GetSelfRow() const
{
    // 如果没有父节点
    if (m_pParentItem == nullptr)
    {
        return 0;
    }

    for (int i = 0; i < m_pParentItem->m_vecChildItems.size(); i++)
    {
        auto pOneChild = m_pParentItem->m_vecChildItems[i];
        if (pOneChild == nullptr)
        {
            continue;
        }

        if (pOneChild->m_strNodeName == m_strNodeName)
        {
            return i;
        }
    }

    return 0;
}

ZKNodeTreeItem* ZKNodeTreeItem::GetParent()
{
    return m_pParentItem;
}

QVariant ZKNodeTreeItem::data(const int nColunm) const
{
    if (nColunm == 0)
    {
        return QString::fromStdString(m_strNodeName);
    }

    return QVariant();
}

const std::string& ZKNodeTreeItem::GetNodeName() const
{
    return m_strNodeName;
}

const std::string& ZKNodeTreeItem::GetFullPath() const
{
    return m_strNodeFullPath;
}

int ZKNodeTreeItem::GetPathDepth() const
{
    return m_nPathDepth;
}

const QVariant& ZKNodeTreeItem::GetNodeValue() const
{
    return m_strNodeValue;
}

void ZKNodeTreeItem::SetNodeValue(const QVariant& strNewData)
{
    m_strNodeValue = strNewData;
}
