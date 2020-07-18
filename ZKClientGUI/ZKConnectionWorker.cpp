#include "ZKConnectionWorker.h"
#include <QTimer>
#include <QThread>
#include <QDebug>

// 转换ZooKeeper状态码到字符串
std::string ZKState2String(int nState)
{
    if (nState == ZOO_NOTCONNECTED_STATE)
        return "ZOO_NOTCONNECTED_STATE";
    if (nState == ZOO_CONNECTING_STATE)
        return "ZOO_CONNECTING_STATE";
    if (nState == ZOO_ASSOCIATING_STATE)
        return "ZOO_ASSOCIATING_STATE";
    if (nState == ZOO_CONNECTED_STATE)
        return "ZOO_CONNECTED_STATE";
    if (nState == ZOO_READONLY_STATE)
        return "ZOO_READONLY_STATE";
    if (nState == ZOO_EXPIRED_SESSION_STATE)
        return "ZOO_EXPIRED_SESSION_STATE";
    if (nState == ZOO_AUTH_FAILED_STATE)
        return "ZOO_AUTH_FAILED_STATE";

    return "INVALID_ZOO_STATE: " + std::to_string(nState);
}

void ZKConnectionStateCallback(zhandle_t* zh, int type,
                               int state, const char* path, void* watcherCtx)
{
    if (type == ZOO_SESSION_EVENT)
    {
        // 记录状态
        ZKConnectionWorker* pWorker = static_cast<ZKConnectionWorker*>(watcherCtx);

        emit pWorker->ZKConnectionStateChanged(state);


        qDebug() << "ZKConnectionStateCallback State: " << QString::fromStdString(ZKState2String(state)) << " " << QThread::currentThreadId();

        if (state == ZOO_CONNECTED_STATE)
        {
            auto pClientID = zoo_client_id(zh);
            if (pClientID != nullptr)
            {
                emit pWorker->ZKClientSessionIDChanged(pClientID->client_id);
            }
        }
    }

    return;
}

void ZKLogCallback(const char* message)
{
    qDebug() << message;
}


ZKConnectionWorker::ZKConnectionWorker(const ZKConnection& xConnection)
    : m_xZKConnectionInfo(xConnection), m_pBuf(new char[1024 * 1024 * 3]), m_nBufLen(1024 * 1024 * 3)
{
    // 使用信号槽机制传输特殊数据类型
    qRegisterMetaType<std::shared_ptr<ZKNodeTreeItem>>("std::shared_ptr<ZKNodeTreeItem>");
    qRegisterMetaType<Stat>("Stat");
    qRegisterMetaType<int64_t>("int64_t");
    qRegisterMetaType<std::string>("std::string");

    connect(this, &ZKConnectionWorker::Start, this, &ZKConnectionWorker::InitWorker);
    connect(this, &ZKConnectionWorker::GetValue, this, &ZKConnectionWorker::TryGetValue);
    connect(this, &ZKConnectionWorker::SetValue, this, &ZKConnectionWorker::TrySetValue);
    connect(this, &ZKConnectionWorker::CreateNode, this, &ZKConnectionWorker::TryCreateNode);
    connect(this, &ZKConnectionWorker::DeleteNode, this, &ZKConnectionWorker::TryDeleteNode);

}

ZKConnectionWorker::~ZKConnectionWorker()
{
    if (m_pHandler != nullptr)
    {
        zookeeper_close(m_pHandler);
        m_pHandler = nullptr;
    }

    if (m_pBuf != nullptr)
    {
        delete[] m_pBuf;
    }
}

QString ZKConnectionWorker::GetZKErrorTip(const ZKOperationType eType, const int rc)
{
    switch (rc)
    {
    case ZNONODE:
        if (eType == ZKOperationType::OPERATION_CREATE)
        {
            return tr("the parent node does not exist.");
        }
        else
        {
            return tr("the node does not exist.");
        }
    case ZNOAUTH:
        return tr("the client does not have permission.");
    case ZBADVERSION:
        return tr("expected version does not match actual version.");
    case ZBADARGUMENTS:
        return tr("invalid input parameters");
    case ZINVALIDSTATE:
        return tr("zhandle state is either ZOO_SESSION_EXPIRED_STATE or ZOO_AUTH_FAILED_STATE");
    case ZMARSHALLINGERROR:
        return tr("failed to marshall a request; possibly, out of memory");
    case ZNODEEXISTS:
        return tr("the node already exists");
    case ZNOCHILDRENFOREPHEMERALS:
        return tr("cannot create children of ephemeral nodes.");
    case ZNOTEMPTY:
        return tr("children are present; node cannot be deleted.");
    default:
        return tr("Unknown zk error: ") + QString::number(rc);
    }
}


void ZKConnectionWorker::InitWorker()
{
    QTimer* pTimer = new QTimer(this);
    pTimer->setInterval(20);
    connect(pTimer, &QTimer::timeout, this, &ZKConnectionWorker::Execute);

    pTimer->start();
}

void ZKConnectionWorker::Execute()
{
    CheckAndInitConnection();
    CheckQueryList();
}

void ZKConnectionWorker::TryGetValue(std::shared_ptr<ZKNodeTreeItem> pTreeItem)
{
    if (pTreeItem == nullptr)
    {
        return;
    }

    bool bNeedRetry = false;
    DoGetValue(pTreeItem.get(), bNeedRetry);

    if (bNeedRetry)
    {
        m_listQuery.push_back(pTreeItem);
    }
    else
    {
        emit AfterGetValue(pTreeItem);
    }
}

void ZKConnectionWorker::TrySetValue(const QString& strPath, const QString& strValue, const int nVersion, const int64_t nTransactionID)
{
    QString strResult;

    if (!IsZKConnected())
    {
        strResult = tr("ZK is not connected.");
        emit AfterSetValue(strPath, strValue, strResult, Stat(), nTransactionID);
        return;
    }

    const std::string stringValue = strValue.toStdString();

    m_xStat = { 0 };

    const QString strFunction = "zoo_set2(" + strPath + ")";

    emit BeforeCallingZKFunction(strFunction);
    const int nSetResult = zoo_set2(m_pHandler, strPath.toStdString().c_str(), stringValue.c_str(), stringValue.length(), nVersion, &m_xStat);
    emit AfterCallingZKFunction(strFunction);
    if (nSetResult == ZOK)
    {
        strResult.clear();
    }
    else
    {
        strResult = GetZKErrorTip(ZKOperationType::OPERATION_SET, nSetResult);
    }

    emit AfterSetValue(strPath, strValue, strResult, m_xStat, nTransactionID);
}

void ZKConnectionWorker::TryCreateNode(const std::string& strPath, const QVariant& strValue, const int nCreateMode, const int64_t nTransactionID, const bool bGoAfterCreated)
{
    QString strResult;

    if (!IsZKConnected())
    {
        strResult = tr("ZK is not connected.");
        emit AfterCreateNode(strPath, strValue, strResult, Stat(), nTransactionID, bGoAfterCreated);
        return;
    }

    std::string stringValue;

    if (strValue.isValid())
    {
        stringValue = strValue.toString().toStdString();
    }

    std::string strRealPath;

    m_xStat = { 0 };

    const QString strFunction = "zoo_create2(" + QString::fromStdString(strPath) + ")";
    emit BeforeCallingZKFunction(strFunction);
    const auto nCreateResult = zoo_create2(m_pHandler, strPath.c_str(), strValue.isValid() ? stringValue.c_str() : nullptr, strValue.isValid() ? stringValue.length() : -1,
                                           &ZOO_OPEN_ACL_UNSAFE, nCreateMode, m_pBuf, m_nBufLen, &m_xStat);
    emit AfterCallingZKFunction(strFunction);
    if (nCreateResult == ZOK)
    {
        strResult.clear();
        strRealPath = std::string(m_pBuf);
    }
    else
    {
        strResult = GetZKErrorTip(ZKOperationType::OPERATION_CREATE, nCreateResult);
    }

    emit AfterCreateNode(strRealPath, strValue, strResult, m_xStat, nTransactionID, bGoAfterCreated);
}

void ZKConnectionWorker::TryDeleteNode(const std::string& strPath, const int nVersion, bool bRecursively, const int64_t nTransactionID)
{
    QString strResult;

    if (!IsZKConnected())
    {
        strResult = tr("ZK is not connected.");
        emit AfterDeleteNode(strPath, strResult, nTransactionID);
        return;
    }

    strResult = DoDeleteNode(strPath, bRecursively, nVersion);

    emit AfterDeleteNode(strPath, strResult, nTransactionID);
}

void ZKConnectionWorker::CheckAndInitConnection()
{
    // 先检查是否有旧连接
    if (m_pHandler != nullptr)
    {
        auto nConnectionState = zoo_state(m_pHandler);

        // 状态正常，不需要重新初始化
        if (nConnectionState != ZOO_EXPIRED_SESSION_STATE && nConnectionState != ZOO_AUTH_FAILED_STATE)
        {
            return;
        }

        // 状态不正常，先关闭
        zookeeper_close(m_pHandler);
        m_pHandler = nullptr;

        // 重置数据
    }

    emit BeforeCallingZKFunction("zookeeper_init2");
    m_pHandler = zookeeper_init2(m_xZKConnectionInfo.strHosts.toStdString().c_str(), ZKConnectionStateCallback, 5000, 0, this, 0, ZKLogCallback);
    emit AfterCallingZKFunction("zookeeper_init2");
    if (m_pHandler == nullptr)
    {
        qDebug() << "Failed to init zk.";
    }
    else
    {
        const auto nState = zoo_state(m_pHandler);
        qDebug() << "CheckAndInitConnection" << QString::fromStdString(ZKState2String(nState));
        emit ZKConnectionStateChanged(nState);
    }

}

void ZKConnectionWorker::CheckQueryList()
{
    if (!IsZKConnected())
    {
        return;
    }

    for (auto it = m_listQuery.begin(); it != m_listQuery.end();)
    {
        bool bNeedRetry = false;
        DoGetValue(it->get(), bNeedRetry);

        if (bNeedRetry)
        {
            it++;
        }
        else
        {
            emit AfterGetValue(*it);
            it = m_listQuery.erase(it);
        }
    }
}

bool ZKConnectionWorker::IsZKConnected()
{
    if (m_pHandler == nullptr)
    {
        return false;
    }

    return zoo_state(m_pHandler) == ZOO_CONNECTED_STATE;
}

void ZKConnectionWorker::DoGetValue(ZKNodeTreeItem* pTreeItem, bool& bRetry)
{
    if (pTreeItem == nullptr)
    {
        return;
    }

    if (!IsZKConnected())
    {
        bRetry = true;
        return;
    }

    m_nReturnBufLen = m_nBufLen;
    m_xStat = { 0 };

    const QString strGetFunction = "zoo_get(" + QString::fromStdString(pTreeItem->GetFullPath()) + ")";
    emit BeforeCallingZKFunction(strGetFunction);
    const auto nGetResult = zoo_get(m_pHandler, pTreeItem->GetFullPath().c_str(), 0, m_pBuf, &m_nReturnBufLen, &m_xStat);
    emit AfterCallingZKFunction(strGetFunction);

    if (nGetResult == ZOK)
    {
        // 值为nullptr
        if (m_nReturnBufLen < 0)
        {
            pTreeItem->SetNodeValue(QVariant());
        }
        else
        {
            pTreeItem->SetNodeValue(QString::fromUtf8(m_pBuf, m_nReturnBufLen));
        }

        pTreeItem->m_xStat = m_xStat;
    }
    else if (nGetResult == ZNONODE)
    {
        pTreeItem->m_bNoNode = true;
        return;
    }
    else if (nGetResult == ZBADARGUMENTS)
    {
        qDebug() << "zoo_get error: " << nGetResult << ". Path: " << pTreeItem->GetFullPath().c_str();
        return;
    }
    else
    {
        bRetry = true;
        return;
    }

    // 子节点先设置为删除状态
    for (int i = 0; i < pTreeItem->GetChildCount(); i++)
    {
        auto pOneChild = pTreeItem->GetChild(i);
        if (pOneChild != nullptr)
        {
            pOneChild->m_bNoNode = true;
        }
    }

    String_vector xStringVector = { 0 };

    const QString strGetChildrenFunction = "zoo_get_children(" + QString::fromStdString(pTreeItem->GetFullPath()) + ")";
    emit BeforeCallingZKFunction(strGetChildrenFunction);
    const auto nChildResult = zoo_get_children(m_pHandler, pTreeItem->GetFullPath().c_str(), 0, &xStringVector);
    emit AfterCallingZKFunction(strGetChildrenFunction);
    if (nChildResult == ZOK)
    {
        for (int i = 0; i < xStringVector.count; i++)
        {
            const std::string strChildName(xStringVector.data[i]);

            auto pFindChild = pTreeItem->GetChildByName(strChildName);
            if (pFindChild == nullptr)
            {
                pFindChild = pTreeItem->AddChild(strChildName, QVariant());
            }

            pFindChild->m_bNoNode = false;

            // 继续查询下下级
            if (pTreeItem->m_bQueryNextLevel)
            {
                DoGetValue(pFindChild, bRetry);
            }
        }

        deallocate_String_vector(&xStringVector);
    }
    else if (nChildResult == ZNONODE)
    {
        pTreeItem->m_bNoNode = true;
    }
    else if (nChildResult == ZBADARGUMENTS)
    {
        qDebug() << "zoo_get_children error: " << nGetResult << ". Path: " << pTreeItem->GetFullPath().c_str();
    }
    else
    {
        bRetry = true;
        return;
    }
}

QString ZKConnectionWorker::DoDeleteNode(const std::string& strPath, bool bRecursively, const int nVersion)
{
    if (bRecursively)
    {
        // 删除子节点

        // 如果要校验版本，先获取一下版本，防止错误删除子节点
        if (nVersion != -1)
        {
            m_nReturnBufLen = m_nBufLen;
            m_xStat = { 0 };

            const QString strFunction = "zoo_get(" + QString::fromStdString(strPath) + ")";
            emit BeforeCallingZKFunction(strFunction);
            const auto nGetResult = zoo_get(m_pHandler, strPath.c_str(), 0, m_pBuf, &m_nReturnBufLen, &m_xStat);
            emit AfterCallingZKFunction(strFunction);
            if (nGetResult == ZOK)
            {
                if (m_xStat.version != nVersion)
                {
                    return GetZKErrorTip(ZKOperationType::OPERATION_GET, ZBADVERSION);
                }
            }
            else
            {
                return GetZKErrorTip(ZKOperationType::OPERATION_GET, nGetResult);
            }
        }

        // 获取子节点
        String_vector xStringVector = { 0 };

        const QString strFunction = "zoo_get_children(" + QString::fromStdString(strPath) + ")";
        emit BeforeCallingZKFunction(strFunction);
        const auto nChildResult = zoo_get_children(m_pHandler, strPath.c_str(), 0, &xStringVector);
        emit AfterCallingZKFunction(strFunction);
        if (nChildResult == ZOK)
        {
            QString strDeleteChildResult;

            for (int i = 0; i < xStringVector.count; i++)
            {
                const std::string strChildName(xStringVector.data[i]);

                std::string strChildPath;
                if (strPath == "/")
                {
                    strChildPath = strPath + strChildName;
                }
                else
                {
                    strChildPath = strPath + "/" + strChildName;
                }


                strDeleteChildResult = DoDeleteNode(strChildPath, bRecursively, -1);

                if (!strDeleteChildResult.isEmpty())
                {
                    break;
                }

            }

            deallocate_String_vector(&xStringVector);

            if (!strDeleteChildResult.isEmpty())
            {
                return strDeleteChildResult;
            }
        }
        else
        {
            return GetZKErrorTip(ZKOperationType::OPERATION_GET_CHILDREN, nChildResult);
        }
    }

    // 删除当前节点
    const QString strFunction = "zoo_delete(" + QString::fromStdString(strPath) + ")";
    emit BeforeCallingZKFunction(strFunction);
    const auto nDeleteResult = zoo_delete(m_pHandler, strPath.c_str(), nVersion);
    emit AfterCallingZKFunction(strFunction);

    if (nDeleteResult == ZOK || nDeleteResult == ZNONODE)
    {
        return QString();
    }
    else
    {
        return GetZKErrorTip(ZKOperationType::OPERATION_DELETE, nDeleteResult);
    }

    return QString();
}
