#pragma once

#include <QDialog>
#include "ui_EditValueDialog.h"
#include <string>
#include "ZKConnectionWorker.h"

class EditValueDialog : public QDialog
{
    Q_OBJECT

public:
    EditValueDialog(QWidget* parent, ZKConnectionWorker* pWorker);
    ~EditValueDialog();

    void SettingConfig(const std::string& strFullPath, const QString& strValue, const int nVersion);

private slots:

    void OnZKReturnSetValueResult(const QString& strPath, const QString& strValue, const QString& strResult, const Stat& xStat, const int64_t nTransactionID);

private:
    Ui::EditValueDialog ui;

    ZKConnectionWorker* const m_pWorker;

    QString m_strFullPath;
    QString m_strValue;
    int m_nVersion;

    int64_t m_nTransactionID = 0; // 每编辑一次，就+1，用来在ZK返回数据的时候验证一下是不是对应这次操作，从而可以选择是否关闭当前窗口
};
