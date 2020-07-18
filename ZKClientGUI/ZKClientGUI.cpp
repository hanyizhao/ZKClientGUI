#include "ZKClientGUI.h"
#include <QMenu>
#include <QDebug>
#include <QMessageBox>
#include "ZKDataViewWidget.h"
#include "Language.h"

ZKClientGUI::ZKClientGUI(QWidget* parent, const int nLanguageIndex)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    if (nLanguageIndex >= 0)
    {
        ui.languageComboBox->setCurrentIndex(nLanguageIndex);
        m_nNowLanguageIndex = nLanguageIndex;
    }
    
    ui.languageRestartLabel->setVisible(false);

    connect(ui.languageComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(OnLanguageChanged(int)));

    m_pConnectionsModel = new EditConnectionsModel(parent);

    ui.tableView->setModel(m_pConnectionsModel);

    QMenu* pMenu = new QMenu(ui.tableView);

    pMenu->addAction(tr("Connect"), this, SLOT(OnConnectButtonClicked()));

    pMenu->addAction(tr("Delete selected items"), [ = ]()->void
    {
        // 获取选择的行
        QModelIndexList listSelected = ui.tableView->selectionModel()->selectedRows();

        std::vector<size_t> vecRows;
        for (auto& xModeIndex : listSelected)
        {
            vecRows.push_back(xModeIndex.row());
        }

        // 从数据模型移除这些行
        m_pConnectionsModel->RemoveItems(vecRows);
    });

    connect(ui.tableView, &QTableView::customContextMenuRequested, [ = ]()->void
    {
        pMenu->exec(QCursor::pos());
    });

    connect(ui.connectButton, SIGNAL(clicked(bool)), this, SLOT(OnConnectButtonClicked(bool)));

    // 点击页签的关闭按钮时的回调函数
    connect(ui.tabWidget, &QTabWidget::tabCloseRequested, [&](int index)
    {
        auto pWidget = ui.tabWidget->widget(index);
        ui.tabWidget->removeTab(index);

        if (pWidget != nullptr)
        {
            delete pWidget;
        }
    });


    // 设置页签都有关闭按钮
    ui.tabWidget->setTabsClosable(true);

    // 因为第一个页签不需要关闭按钮，因此手动去除
    ui.tabWidget->tabBar()->setTabButton(ui.tabWidget->indexOf(ui.hostsTab), QTabBar::RightSide, nullptr);

    ui.tableView->horizontalHeader()->setStretchLastSection(true);
}

void ZKClientGUI::OnLanguageChanged(int nIndex)
{
    Language::SaveLanguageToFile(nIndex);
	ui.languageRestartLabel->setVisible(m_nNowLanguageIndex != nIndex);
}

void ZKClientGUI::OnConnectButtonClicked(bool)
{
    // 获取选择的行
    QModelIndexList listSelected = ui.tableView->selectionModel()->selectedRows();

    if (listSelected.empty())
    {
        QMessageBox::information(this, tr("Info"), tr("Please edit connection detail and choose one connection firstly."));
        return;
    }

    ZKConnection xConnectionInfo;

    if (!m_pConnectionsModel->GetConnectionInfoByRow(listSelected.first().row(), xConnectionInfo))
    {
        QMessageBox::warning(this, tr("Warn"), tr("Failed to get zookeeper connection configuration."));
        return;
    }

    const int nMaxTabLength = 35; // 最多这么多个字符

    QString strTabTitle = xConnectionInfo.strName + ": " + xConnectionInfo.strHosts;
    if (strTabTitle.length() > nMaxTabLength)
    {
        strTabTitle = strTabTitle.left(nMaxTabLength) + "...";
    }

    ZKDataViewWidget* pViewWidget = new ZKDataViewWidget(ui.tabWidget, xConnectionInfo);
    ui.tabWidget->addTab(pViewWidget, strTabTitle);
    ui.tabWidget->setCurrentWidget(pViewWidget);
}