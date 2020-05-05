#pragma once

#include <QAbstractTableModel>
#include "ZKConnection.h"

class EditConnectionsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    EditConnectionsModel(QObject* parent = nullptr);
    virtual ~EditConnectionsModel();

    // QAbstractTableModel 中的方法
    virtual Qt::ItemFlags flags(const QModelIndex& index) const override;
    Q_INVOKABLE virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    Q_INVOKABLE virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    Q_INVOKABLE virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    Q_INVOKABLE virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // 删除选中的项目
    void RemoveItems(const std::vector<size_t>& vecRows);

    bool GetConnectionInfoByRow(const int nRow, ZKConnection& xOutConInfo);

private:
    // 从本地存储读取连接配置，然后初始化 m_vecConnections
    void InitConnectionsFromLocalConfig();

    // 将 m_vecConnections写回到本地配置文件
    void SaveConnectionsToLocalConfig();

private:
    std::vector<ZKConnection> m_vecConnections;

    std::vector<QString> m_vecHeaders = { tr("Connection Name"),  tr("Hosts") };

};

