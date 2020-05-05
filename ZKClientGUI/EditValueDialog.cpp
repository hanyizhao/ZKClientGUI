#include "EditValueDialog.h"

EditValueDialog::EditValueDialog(QWidget* parent, ZKConnectionWorker* pWorker)
    : QDialog(parent), m_pWorker(pWorker)
{
    ui.setupUi(this);

    connect(pWorker, &ZKConnectionWorker::AfterSetValue, this, &EditValueDialog::OnZKReturnSetValueResult);

    connect(ui.editButton, &QPushButton::clicked, [&](bool)
    {
        emit m_pWorker->SetValue(m_strFullPath, ui.plainTextEdit->toPlainText(), ui.versionCheckBox->isChecked() ? m_nVersion : -1, m_nTransactionID);
        ui.editButton->setEnabled(false);
    });

    connect(ui.cancelButton, &QPushButton::clicked, [&](bool)
    {
        this->hide();
    });
}

EditValueDialog::~EditValueDialog()
{
}

void EditValueDialog::SettingConfig(const std::string& strFullPath, const QString& strValue, const int nVersion)
{
    m_nTransactionID++;

    m_strFullPath = QString::fromStdString(strFullPath);
    m_strValue = strValue;
    m_nVersion = nVersion;
    ui.lineEdit->setText(QString::fromStdString(strFullPath));
    ui.plainTextEdit->setPlainText(strValue);
    ui.versionCheckBox->setText(tr("Version Check ") + "(" + tr("current version is ") + QString::number(nVersion) + ")");

    ui.editButton->setEnabled(true);
}

void EditValueDialog::OnZKReturnSetValueResult(const QString& strPath, const QString& strValue, const QString& strResult, const Stat& xStat, const int64_t nTransactionID)
{
    if (nTransactionID != m_nTransactionID)
    {
        return;
    }

    if (this->isHidden())
    {
        return;
    }

    // 成功
    if (strResult.isEmpty())
    {
        this->hide();
    }
    // 失败，允许再次提交
    else
    {
        ui.editButton->setEnabled(true);
    }
}
