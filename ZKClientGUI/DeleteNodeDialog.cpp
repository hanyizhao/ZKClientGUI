#include "DeleteNodeDialog.h"

DeleteNodeDialog::DeleteNodeDialog(QWidget* parent, ZKConnectionWorker* pWorker)
    : QDialog(parent), m_pWorker(pWorker)
{
    ui.setupUi(this);

    connect(m_pWorker, &ZKConnectionWorker::AfterDeleteNode, this, &DeleteNodeDialog::OnZKReturnDeleteNodeResult);
    connect(ui.cancelButton, &QPushButton::clicked, [&](bool)
    {
        this->hide();
    });

    connect(ui.deleteButton, &QPushButton::clicked, [&](bool)
    {
        emit m_pWorker->DeleteNode(m_strPath, ui.versionCheckBox->isChecked() ? m_nVersion : -1,
                                   ui.recursiveCheckBox->isChecked(), m_nTransactionID);
        ui.deleteButton->setEnabled(false);
    });
}

DeleteNodeDialog::~DeleteNodeDialog()
{
}

void DeleteNodeDialog::SetDeletePath(const std::string& strPath, int nVersion)
{
    m_nTransactionID++;

    m_strPath = strPath;
    m_nVersion = nVersion;
    ui.pathLineEdit->setText(QString::fromStdString(strPath));
    ui.versionCheckBox->setText(tr("Version Check") + " (" + tr("current version is ") + QString::number(nVersion) + ")");
    ui.deleteButton->setEnabled(true);
}

void DeleteNodeDialog::OnZKReturnDeleteNodeResult(const std::string& strPath, const QString& strResult, const int64_t nTransactionID)
{
    if (nTransactionID != m_nTransactionID)
    {
        return;
    }

    if (this->isHidden())
    {
        return;
    }

    ui.deleteButton->setEnabled(true);

    // 成功
    if (strResult.isEmpty())
    {
        this->hide();
    }
}
