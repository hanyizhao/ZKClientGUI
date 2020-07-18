#pragma once

#include <QObject>
#include "ZKConnection.h"
#include "zookeeper.h"
#include <memory>
#include "ZKNodeTreeItem.h"

class ZKConnectionWorker : public QObject
{
    Q_OBJECT

public:
    ZKConnectionWorker(const ZKConnection& xConnection);
    ~ZKConnectionWorker();

signals:

    void BeforeCallingZKFunction(const QString& strFunc);
    void AfterCallingZKFunction(const QString& strFunc);

    // 连接状态变化信号
    void ZKConnectionStateChanged(int);

    // 当前的客户端Session变化
    void ZKClientSessionIDChanged(int64_t);

    // 初始化Worker
    void Start();

    // 获取树中所有节点的当前状态和值，并且获取下一级的状态和值
    void GetValue(std::shared_ptr<ZKNodeTreeItem> pTreeItem);

    void AfterGetValue(std::shared_ptr<ZKNodeTreeItem> pTreeItem);

    void SetValue(const QString& strPath, const QString& strValue, const int nVersion, const int64_t nTransactionID);

    void AfterSetValue(const QString& strPath, const QString& strValue, const QString& strResult, const Stat& xStat, const int64_t nTransactionID);

    void CreateNode(const std::string& strPath, const QVariant& strValue, const int nCreateMode, const int64_t nTransactionID, const bool bGoAfterCreated);

    void AfterCreateNode(const std::string& strRealPath, const QVariant& strValue, const QString& strResult, const Stat& xStat, const int64_t nTransactionID, const bool bGoAfterCreated);

    void DeleteNode(const std::string& strPath, const int nVersion, bool bRecursively, const int64_t nTransactionID);

    void AfterDeleteNode(const std::string& strPath, const QString& strResult, const int64_t nTransactionID);

private slots:

    void InitWorker();

    void Execute();

    void TryGetValue(std::shared_ptr<ZKNodeTreeItem> pTreeItem);

    void TrySetValue(const QString& strPath, const QString& strValue, const int nVersion, const int64_t nTransactionID);

    void TryCreateNode(const std::string& strPath, const QVariant& strValue, const int nCreateMode, const int64_t nTransactionID, const bool bGoAfterCreated);

    void TryDeleteNode(const std::string& strPath, const int nVersion, bool bRecursively, const int64_t nTransactionID);

private:
    enum class ZKOperationType;
    QString GetZKErrorTip(const ZKOperationType eType, const int rc);

    // 尝试连接ZK
    void CheckAndInitConnection();

    // 检查未完成的查询任务
    void CheckQueryList();

    // ZK是否连接成功
    bool IsZKConnected();

    // 进行查询
    void DoGetValue(ZKNodeTreeItem* pTreeItem, bool& bRetry);

    QString DoDeleteNode(const std::string& strPath, bool bRecursively, const int nVersion);

private:
    enum class ZKOperationType
    {
        OPERATION_GET,
        OPERATION_GET_CHILDREN,
        OPERATION_CREATE,
        OPERATION_SET,
        OPERATION_DELETE
    };

    const ZKConnection m_xZKConnectionInfo; // ZK连接信息
    zhandle_t* m_pHandler = nullptr; // ZK handler
    std::list<std::shared_ptr<ZKNodeTreeItem>> m_listQuery; // 保存还没有完成的查询任务

    char* const m_pBuf; // 查询时使用的Buf，因为单线程读写，因此可以共用一个
    const int m_nBufLen; // Buf长度
    int m_nReturnBufLen = 0;
    Stat m_xStat = {0};
};
