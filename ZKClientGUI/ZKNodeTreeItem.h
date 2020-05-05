#pragma once
#include <QVector>
#include <string>
#include <QVariant>
#include "zookeeper.h"

class ZKNodeTreeItem
{
public:
    explicit ZKNodeTreeItem(ZKNodeTreeItem* pParentItem, const std::string& strParentPath, const std::string& strNodeName,
                            const QVariant& strNodeValue, int nPathDepth);

    // 复制一棵树
    ZKNodeTreeItem(const ZKNodeTreeItem& xSource) = delete;

    virtual ~ZKNodeTreeItem();

    void SetParent(ZKNodeTreeItem* pParentItem);
    ZKNodeTreeItem* AddChild(const std::string& strNodeName, const QVariant& strNodeValue);
    ZKNodeTreeItem* GetChild(const int nRow) const;
    ZKNodeTreeItem* GetChildByName(const std::string& strName) const;
    void RemoveChild(const std::string& strName);
    int GetChildCount() const;
    int GetSelfRow() const;
    ZKNodeTreeItem* GetParent();
    QVariant data(const int nColunm) const;

    const std::string& GetNodeName() const;
    const std::string& GetFullPath() const;
    int GetPathDepth() const;
    const QVariant& GetNodeValue() const;

    void SetNodeValue(const QVariant& strNewData);

public:
    bool m_bQueryNextLevel = false; // ZK线程查询的时候使用。表示：是否继续查询下下级。默认只查询下一级。True的时候查询下下级
    bool m_bNoNode = false; // ZK线程查询的时候使用。表示节点已经不存在了
    Stat m_xStat; // 节点状态

private:
    QVector<ZKNodeTreeItem*> m_vecChildItems; // 子节点
    ZKNodeTreeItem* m_pParentItem;   // 父节点

    const int m_nPathDepth; // 路径深度，"/"的深度是0
    const std::string m_strNodeFullPath; // 完整路径
    const std::string m_strNodeName; // 节点名称
    QVariant m_strNodeValue; // 节点值，可能为空

};