#pragma once

#include <QDialog>
#include "ui_CreateChildNodeDialog.h"
#include "ZKConnectionWorker.h"

class CreateChildNodeDialog : public QDialog
{
    Q_OBJECT

public:
    CreateChildNodeDialog(QWidget* parent, ZKConnectionWorker* pWorker);
    ~CreateChildNodeDialog();

    void SetParentPath(const std::string& strParentPath);

private slots:

    void OnCreateButtonClicked(bool);

    void OnZKReturnCreateNodeResult(const std::string& strRealPath, const QVariant& strValue, const QString& strResult, const Stat& xStat,
                                    const int64_t nTransactionID, const bool bGoAfterCreated);

private:
    Ui::CreateChildNodeDialog ui;

    ZKConnectionWorker* const m_pWorker;

    std::string m_strParentPath;
    int64_t m_nTransactionID = 0;
};
