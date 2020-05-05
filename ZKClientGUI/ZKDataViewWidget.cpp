#include "ZKDataViewWidget.h"
#include <QDebug>
#include <QThread>
#include <QMessageBox>
#include <QDateTime>
#include <QMenu>

constexpr auto strTimeFormat = "yyyy-MM-dd HH:mm:ss.zzz";

ZKDataViewWidget::ZKDataViewWidget(QWidget* parent, const ZKConnection& xCon)
    : QWidget(parent), m_pTreeModel(new ZKNodeTreeModel(this))
{
    ui.setupUi(this);

    //ui.splitter->setStretchFactor(0, 3);
    //ui.splitter->setStretchFactor(1, 7);

    ///////// 线程相关 ////////////////////
    m_pWorkingThread = new QThread;
    m_pZKConnectionWorker = new ZKConnectionWorker(xCon);
    m_pZKConnectionWorker->moveToThread(m_pWorkingThread);
    // 线程运行结束后，会自动删除自己
    connect(m_pWorkingThread, &QThread::finished, m_pZKConnectionWorker, &QObject::deleteLater);
    connect(m_pWorkingThread, &QThread::finished, m_pWorkingThread, &QObject::deleteLater);
    m_pWorkingThread->start();

    connect(m_pZKConnectionWorker, &ZKConnectionWorker::BeforeCallingZKFunction, this, &ZKDataViewWidget::OnBeforeCallingZKFunction);
    connect(m_pZKConnectionWorker, &ZKConnectionWorker::AfterCallingZKFunction, this, &ZKDataViewWidget::OnAfterCallingZKFunction);
    connect(m_pZKConnectionWorker, &ZKConnectionWorker::ZKClientSessionIDChanged, this, &ZKDataViewWidget::OnZKClientSessionIDChanged);
    connect(m_pZKConnectionWorker, &ZKConnectionWorker::ZKConnectionStateChanged, this, &ZKDataViewWidget::DoZKStateChanged);
    connect(m_pZKConnectionWorker, &ZKConnectionWorker::AfterGetValue, this, &ZKDataViewWidget::OnZKReturnQueryResult);
    connect(m_pZKConnectionWorker, &ZKConnectionWorker::AfterSetValue, this, &ZKDataViewWidget::OnZKReturnSetValueResult);
    connect(m_pZKConnectionWorker, &ZKConnectionWorker::AfterCreateNode, this, &ZKDataViewWidget::OnZKReturnCreateNodeResult);
    connect(m_pZKConnectionWorker, &ZKConnectionWorker::AfterDeleteNode, this, &ZKDataViewWidget::OnZKReturnDeleteNodeResult);

    emit m_pZKConnectionWorker->Start();

    connect(ui.treeView, &QTreeView::expanded, this, &ZKDataViewWidget::OnTreeViewExpanded);

    ui.treeView->setModel(m_pTreeModel);

    // 效果：QTreeView出现横向滚动条，从而可以展示完整的内容
    // 如果不这样设置，会导致在面板比较窄的时候，内容被截短、出现"..."
    //
    // 1. 设置QTreeView的headerStretchLastSection 为false（Qt Designer中操作）
    // 2. 下面一行
    ui.treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui.treeView->expandToDepth(0);
    connect(ui.treeView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &ZKDataViewWidget::OnTreeViewCurrentChanged);

    m_pEditValueDialog = new EditValueDialog(this, m_pZKConnectionWorker);
    connect(ui.editValueButton, &QPushButton::clicked, this, &ZKDataViewWidget::OnEditButtonClicked);


    m_pCreateChildNodeDialog = new CreateChildNodeDialog(this, m_pZKConnectionWorker);
    m_pDeleteNodeDialog = new DeleteNodeDialog(this, m_pZKConnectionWorker);

    QMenu* pMenu = new QMenu(ui.treeView);
    pMenu->addAction(tr("Create New Child"), this, &ZKDataViewWidget::OnCreateNewChildItemChoosed);
    pMenu->addAction(tr("Delete This Node"), this, &ZKDataViewWidget::OnDeleteNodeItemChoosed);
    connect(ui.treeView, &QWidget::customContextMenuRequested, [ = ]()->void
    {
        pMenu->exec(QCursor::pos());
    });
}

ZKDataViewWidget::~ZKDataViewWidget()
{
    // 标记线程可以停止了
    // 停止后会自动释放内存
    m_pWorkingThread->quit();
}

void ZKDataViewWidget::OnBeforeCallingZKFunction(const QString& strFunc)
{
    ui.functionLabel->setText(strFunc);
}

void ZKDataViewWidget::OnAfterCallingZKFunction(const QString& strFunc)
{
    ui.functionLabel->setText(tr("Done"));
}

void ZKDataViewWidget::OnTreeViewCurrentChanged(const QModelIndex& xCurrentIndex, const QModelIndex& xPreviousIndex)
{
    if (!xCurrentIndex.isValid())
    {
        return;
    }

    auto pTreeItem = static_cast<ZKNodeTreeItem*>(xCurrentIndex.internalPointer());
    if (pTreeItem == nullptr)
    {
        return;
    }

    ui.plainTextEdit->setPlainText(pTreeItem->GetNodeValue().toString());
    ui.nullLabel->setVisible(!pTreeItem->GetNodeValue().isValid());

    ui.LastModifiedTimelineEdit->setText(QDateTime::fromMSecsSinceEpoch(pTreeItem->m_xStat.mtime).toString(strTimeFormat));
    ui.createdTimelineEdit->setText(QDateTime::fromMSecsSinceEpoch(pTreeItem->m_xStat.ctime).toString(strTimeFormat));
    ui.versionLineEdit->setText(QString::number(pTreeItem->m_xStat.version));
    ui.CVersionLineEdit->setText(QString::number(pTreeItem->m_xStat.cversion));
    ui.AVersionLineEdit->setText(QString::number(pTreeItem->m_xStat.aversion));
    ui.mzxidLineEdit->setText(QString::number(pTreeItem->m_xStat.mzxid));
    ui.pzxidLineEdit->setText(QString::number(pTreeItem->m_xStat.pzxid));
    ui.czxidLineEdit->setText(QString::number(pTreeItem->m_xStat.czxid));
    ui.numChildrenLineEdit->setText(QString::number(pTreeItem->m_xStat.numChildren));
    ui.dataLengthLineEdit->setText(QString::number(pTreeItem->m_xStat.dataLength));
    ui.NodeFullPathLineEdit->setText(QString::fromStdString(pTreeItem->GetFullPath()));
    ui.ephemeralOwnerLineEdit->setText(QString::number(pTreeItem->m_xStat.ephemeralOwner));
}

void ZKDataViewWidget::OnTreeViewExpanded(const QModelIndex& index)
{
    if (!index.isValid())
    {
        return;
    }

    auto pTreeItem = static_cast<ZKNodeTreeItem*>(index.internalPointer());
    if (pTreeItem == nullptr)
    {
        return;
    }

    auto pParentTreeItem = pTreeItem->GetParent();
    if (pParentTreeItem == nullptr)
    {
        QMessageBox::critical(this, tr("Error"), tr("Parent Node is nullptr. Now Path: ") + QString::fromStdString(pTreeItem->GetFullPath()));
        return;
    }

    auto pQueryItem = std::make_shared<ZKNodeTreeItem>(nullptr, pParentTreeItem->GetFullPath(), pTreeItem->GetNodeName(),
                      pTreeItem->GetNodeValue(), pTreeItem->GetPathDepth());
    pQueryItem->m_bQueryNextLevel = true;

    CheckTreeItemExpanded(pQueryItem.get(), index);

    // 发送给ZK工作线程
    emit m_pZKConnectionWorker->GetValue(pQueryItem);
}

void ZKDataViewWidget::DoZKStateChanged(int nZKState)
{
    qDebug() << "DoZKStateChanged " << nZKState << " " << QThread::currentThreadId();

    // 状态不同，页面底部显示的颜色和提示语不同
    if (nZKState == ZOO_CONNECTED_STATE)
    {
        ui.circlelabel->setStyleSheet("color:green;");
        ui.connectionStatelabel->setText(tr("Connected"));
    }
    else if (nZKState == ZOO_EXPIRED_SESSION_STATE || nZKState == ZOO_AUTH_FAILED_STATE)
    {
        ui.circlelabel->setStyleSheet("color:red;");
        ui.connectionStatelabel->setText(tr("Error"));
    }
    else
    {
        ui.circlelabel->setStyleSheet("color:blue;");
        ui.connectionStatelabel->setText(tr("Connecting..."));
    }
}

void ZKDataViewWidget::OnZKClientSessionIDChanged(int64_t nNowID)
{
    ui.sessionLineEdit->setText(QString::number(nNowID));
}

void ZKDataViewWidget::OnZKReturnQueryResult(std::shared_ptr<ZKNodeTreeItem> pTreeItem)
{
    m_pTreeModel->OnUpdateSubTreeSubValue(pTreeItem);

    OnTreeViewCurrentChanged(ui.treeView->currentIndex(), QModelIndex());
}

void ZKDataViewWidget::OnEditButtonClicked(bool)
{
    auto xIndex = ui.treeView->currentIndex();
    if (!xIndex.isValid())
    {
        return;
    }

    auto pTreeItem = static_cast<ZKNodeTreeItem*>(xIndex.internalPointer());
    if (pTreeItem == nullptr)
    {
        return;
    }

    m_pEditValueDialog->SettingConfig(pTreeItem->GetFullPath(), pTreeItem->GetNodeValue().toString(), pTreeItem->m_xStat.version);
    m_pEditValueDialog->setWindowModality(Qt::WindowModality::WindowModal);
    m_pEditValueDialog->show();
}

void ZKDataViewWidget::OnZKReturnSetValueResult(const QString& strPath, const QString& strValue, const QString& strResult, const Stat& xStat, const int64_t nTransactionID)
{
    if (!strResult.isEmpty())
    {
        QMessageBox::critical(this, "Error", strResult);
        return;
    }

    auto pTreeItem = m_pTreeModel->FindTreeItem(strPath.toStdString(), false);
    if (pTreeItem == nullptr)
    {
        return;
    }

    pTreeItem->SetNodeValue(strValue);
    pTreeItem->m_xStat = xStat;

    emit m_pTreeModel->layoutChanged();
    OnTreeViewCurrentChanged(ui.treeView->currentIndex(), QModelIndex());

    QMessageBox::information(this, tr("info"), tr("Save value successfully."));
}

void ZKDataViewWidget::OnCreateNewChildItemChoosed()
{
    auto xIndex = ui.treeView->currentIndex();

    if (!xIndex.isValid())
    {
        return;
    }

    auto pTreeItem = static_cast<ZKNodeTreeItem*>(xIndex.internalPointer());
    if (pTreeItem == nullptr)
    {
        return;
    }
    m_pCreateChildNodeDialog->SetParentPath(pTreeItem->GetFullPath());
    m_pCreateChildNodeDialog->setWindowModality(Qt::WindowModality::WindowModal);
    m_pCreateChildNodeDialog->show();
}

void ZKDataViewWidget::OnZKReturnCreateNodeResult(const std::string& strRealPath, const QVariant& strValue, const QString& strResult, const Stat& xStat,
        const int64_t nTransactionID, const bool bGoAfterCreated)
{
    if (!strResult.isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), strResult);
        return;
    }

    const auto nSlashPosition = strRealPath.find_last_of('/');
    if (nSlashPosition == std::string::npos)
    {
        return;
    }

    std::string strParentPath = strRealPath.substr(0, nSlashPosition);
    std::string strNodeName = strRealPath.substr(nSlashPosition + 1);

    if (strParentPath.empty())
    {
        strParentPath = "/";
    }

    auto pParentItem = m_pTreeModel->FindTreeItem(strParentPath, false);
    if (pParentItem == nullptr)
    {
        return;
    }

    auto pChild = pParentItem->AddChild(strNodeName, strValue);
    if (pChild != nullptr)
    {
        pChild->m_xStat = xStat;
    }

    emit m_pTreeModel->layoutChanged();
    OnTreeViewCurrentChanged(ui.treeView->currentIndex(), QModelIndex());

    QModelIndex xParentIndex = m_pTreeModel->CarefullyCreateIndex(pParentItem);
    OnTreeViewExpanded(xParentIndex);

    if (bGoAfterCreated)
    {
        if (pChild != nullptr)
        {
            ui.treeView->setCurrentIndex(m_pTreeModel->CarefullyCreateIndex(pChild));
        }
    }

    QMessageBox::information(this, tr("info"), tr("Create node successfully.") + "\n" + QString::fromStdString(strRealPath));
}

void ZKDataViewWidget::OnDeleteNodeItemChoosed()
{
    auto xIndex = ui.treeView->currentIndex();

    if (!xIndex.isValid())
    {
        return;
    }

    auto pTreeItem = static_cast<ZKNodeTreeItem*>(xIndex.internalPointer());
    if (pTreeItem == nullptr)
    {
        return;
    }
    m_pDeleteNodeDialog->SetDeletePath(pTreeItem->GetFullPath(), pTreeItem->m_xStat.version);
    m_pDeleteNodeDialog->setWindowModality(Qt::WindowModality::WindowModal);
    m_pDeleteNodeDialog->show();
}

void ZKDataViewWidget::OnZKReturnDeleteNodeResult(const std::string& strPath, const QString& strResult, const int64_t nTransactionID)
{
    if (!strResult.isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), strResult);
        return;
    }

    auto pTreeItem = m_pTreeModel->FindTreeItem(strPath, false);

    if (pTreeItem == nullptr)
    {
        return;
    }

    auto pParent = pTreeItem->GetParent();
    if (pParent == nullptr)
    {
        return;
    }

    pParent->RemoveChild(pTreeItem->GetNodeName());

    emit m_pTreeModel->layoutChanged();
    OnTreeViewCurrentChanged(ui.treeView->currentIndex(), QModelIndex());

    QModelIndex xParentIndex = m_pTreeModel->CarefullyCreateIndex(pParent);
    OnTreeViewExpanded(xParentIndex);

    QMessageBox::information(this, tr("info"), tr("Delete node successfully.") + "\n" + QString::fromStdString(strPath));
}

void ZKDataViewWidget::CheckTreeItemExpanded(ZKNodeTreeItem* pQueryItem, const QModelIndex& xSourceItemIndex)
{
    if (pQueryItem == nullptr || !xSourceItemIndex.isValid())
    {
        return;
    }

    const auto nRowCount = m_pTreeModel->rowCount(xSourceItemIndex);

    for (int i = 0; i < nRowCount; i++)
    {
        const auto xChildIndex = m_pTreeModel->index(i, 0, xSourceItemIndex);

        if (!ui.treeView->isExpanded(xChildIndex))
        {
            continue;
        }

        auto pChildItem = static_cast<ZKNodeTreeItem*>(xChildIndex.internalPointer());
        if (pChildItem == nullptr)
        {
            continue;
        }

        auto pNewChild = pQueryItem->AddChild(pChildItem->GetNodeName(), pChildItem->GetNodeValue());
        if (pNewChild != nullptr)
        {
            pNewChild->m_bQueryNextLevel = true;

            CheckTreeItemExpanded(pNewChild, xChildIndex);
        }
    }
}

