#pragma once

#include <QDialog>
#include "ui_DeleteNodeDialog.h"
#include "ZKConnectionWorker.h"

class DeleteNodeDialog : public QDialog
{
    Q_OBJECT

public:
    DeleteNodeDialog(QWidget* parent, ZKConnectionWorker* pWorker);
    ~DeleteNodeDialog();

    void SetDeletePath(const std::string& strPath, int nVersion);

private slots:
    void OnZKReturnDeleteNodeResult(const std::string& strPath, const QString& strResult, const int64_t nTransactionID);

private:
    Ui::DeleteNodeDialog ui;

    ZKConnectionWorker* const m_pWorker;
    std::string m_strPath;
    int m_nVersion = 0;
    int64_t m_nTransactionID = 0;
};
