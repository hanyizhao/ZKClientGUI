#include "CreateChildNodeDialog.h"
#include <QMessageBox>

CreateChildNodeDialog::CreateChildNodeDialog(QWidget* parent, ZKConnectionWorker* pWorker)
    : QDialog(parent), m_pWorker(pWorker)
{
    ui.setupUi(this);

    connect(pWorker, &ZKConnectionWorker::AfterCreateNode, this, &CreateChildNodeDialog::OnZKReturnCreateNodeResult);

    connect(ui.createButton, &QPushButton::clicked, this, &CreateChildNodeDialog::OnCreateButtonClicked);

    connect(ui.nullDataCheckBox, &QCheckBox::stateChanged, [ & ](int nState)
    {
        ui.dataTextEdit->setEnabled(nState == Qt::Unchecked);
    });

    connect(ui.cancelButton, &QPushButton::clicked, [&](bool)
    {
        this->hide();
    });

    void (QComboBox::*signalIndexChanged)(int) = &QComboBox::currentIndexChanged;

    connect(ui.createModeComboBox, signalIndexChanged, [&](int nCurrentIndex)
    {
        ui.sequenceCheckBox->setVisible(nCurrentIndex != 2);
    });
}

CreateChildNodeDialog::~CreateChildNodeDialog()
{
}

void CreateChildNodeDialog::SetParentPath(const std::string& strParentPath)
{
    m_nTransactionID++;

    m_strParentPath = strParentPath;

    this->setWindowTitle(tr("Create Child For Node: ") + QString::fromStdString(m_strParentPath));
    ui.dataTextEdit->setPlainText(QString());
    ui.childNameLineEdit->setText(QString());
    ui.createButton->setEnabled(true);
}

void CreateChildNodeDialog::OnCreateButtonClicked(bool)
{
    const auto strNodeName = ui.childNameLineEdit->text().toStdString();
    if (strNodeName.empty())
    {
        QMessageBox::critical(this, tr("Error"), tr("Please enter the node name."));
        return;
    }

    if (strNodeName.find('/') != std::string::npos)
    {
        QMessageBox::critical(this, tr("Error"), tr("Node name must not contain '/'."));
        return;
    }

    std::string strPath;
    if (m_strParentPath == "/")
    {
        strPath = m_strParentPath + strNodeName;
    }
    else
    {
        strPath = m_strParentPath + "/" + strNodeName;
    }

    QVariant strValue;
    if (!ui.nullDataCheckBox->isChecked())
    {
        strValue = ui.dataTextEdit->toPlainText();
    }

    const auto nCreateModeIndex = ui.createModeComboBox->currentIndex();
    int nCreateMode;
    if (nCreateModeIndex == 0)
    {
        if (ui.sequenceCheckBox->isChecked())
        {
            nCreateMode = ZOO_PERSISTENT_SEQUENTIAL;
        }
        else
        {
            nCreateMode = ZOO_PERSISTENT;
        }
    }
    else if (nCreateModeIndex == 1)
    {
        if (ui.sequenceCheckBox->isChecked())
        {
            nCreateMode = ZOO_EPHEMERAL_SEQUENTIAL;
        }
        else
        {
            nCreateMode = ZOO_EPHEMERAL;
        }
    }
    else
    {
        nCreateMode = ZOO_CONTAINER;
    }

    emit m_pWorker->CreateNode(strPath, strValue, nCreateMode, m_nTransactionID, ui.goCheckBox->isChecked());
    ui.createButton->setEnabled(false);
}

void CreateChildNodeDialog::OnZKReturnCreateNodeResult(const std::string& strRealPath, const QVariant& strValue, const QString& strResult, const Stat& xStat,
        const int64_t nTransactionID, const bool bGoAfterCreated)
{
    if (nTransactionID != m_nTransactionID)
    {
        return;
    }

    if (this->isHidden())
    {
        return;
    }

    ui.createButton->setEnabled(true);

    // 成功
    if (strResult.isEmpty())
    {
        if (ui.closeCheckBox->isChecked())
        {
            this->hide();
        }
    }
}
