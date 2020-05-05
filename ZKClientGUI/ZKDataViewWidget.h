#pragma once

#include <QWidget>
#include "ui_ZKDataViewWidget.h"
#include "ZKNodeTreeModel.h"
#include "ZKConnectionWorker.h"
#include <atomic>
#include <QThread>
#include "EditValueDialog.h"
#include "CreateChildNodeDialog.h"
#include "DeleteNodeDialog.h"

//struct ZKCompletionData
//{
//    ZKDataViewWidget* pThis = nullptr;
//    zhandle_t* pHandler = nullptr;
//    std::string strPath;
//};

struct ZKGetValueResult
{
    int nResult = 0;
    std::string strValue;
};

class ZKDataViewWidget : public QWidget
{
    Q_OBJECT

public:
    ZKDataViewWidget(QWidget* parent, const ZKConnection& xCon);
    ~ZKDataViewWidget();

private slots:

    void OnBeforeCallingZKFunction(const QString& strFunc);
    void OnAfterCallingZKFunction(const QString& strFunc);

    void OnTreeViewCurrentChanged(const QModelIndex& xCurrentIndex, const QModelIndex& xPreviousIndex);

    void OnTreeViewExpanded(const QModelIndex& index);

    void DoZKStateChanged(int);

    void OnZKClientSessionIDChanged(int64_t nNowID);

    // ZK线程返回查询结果
    void OnZKReturnQueryResult(std::shared_ptr<ZKNodeTreeItem> pTreeItem);

    void OnEditButtonClicked(bool);

    void OnZKReturnSetValueResult(const QString& strPath, const QString& strValue, const QString& strResult, const Stat& xStat, const int64_t nTransactionID);

    void OnCreateNewChildItemChoosed();

    void OnZKReturnCreateNodeResult(const std::string& strRealPath, const QVariant& strValue, const QString& strResult, const Stat& xStat, const int64_t nTransactionID,
                                    const bool bGoAfterCreated);
    void OnDeleteNodeItemChoosed();

    void OnZKReturnDeleteNodeResult(const std::string& strPath, const QString& strResult, const int64_t nTransactionID);

private:
    void CheckTreeItemExpanded(ZKNodeTreeItem* pQueryItem, const QModelIndex& xSourceItemIndex);

private:
    Ui::ZKDataViewWidget ui;

    ZKNodeTreeModel* m_pTreeModel;

    QThread* m_pWorkingThread = nullptr;
    ZKConnectionWorker* m_pZKConnectionWorker = nullptr;

    EditValueDialog* m_pEditValueDialog = nullptr;
    CreateChildNodeDialog* m_pCreateChildNodeDialog = nullptr;
    DeleteNodeDialog* m_pDeleteNodeDialog = nullptr;
};
