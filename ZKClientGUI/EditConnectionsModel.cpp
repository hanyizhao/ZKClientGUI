#include "EditConnectionsModel.h"
#include <set>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QMessageBox>

// 获取连接配置文件位置
static QString GetConnectionConfigFilePath()
{
    return QCoreApplication::applicationDirPath() + "/connections.json";
}

EditConnectionsModel::EditConnectionsModel(QObject* parent /*= nullptr*/)
    : QAbstractTableModel(parent)
{
    InitConnectionsFromLocalConfig();
}

EditConnectionsModel::~EditConnectionsModel()
{
}

Qt::ItemFlags EditConnectionsModel::flags(const QModelIndex& index) const
{
    auto xFlags = QAbstractTableModel::flags(index);

    xFlags |= Qt::ItemIsEditable; // 表示这一个单元格可以编辑

    return xFlags;

}

Q_INVOKABLE int EditConnectionsModel::rowCount(const QModelIndex& parent) const
{
    return m_vecConnections.size() + 1;
}

Q_INVOKABLE int EditConnectionsModel::columnCount(const QModelIndex& parent) const
{
    return m_vecHeaders.size();
}

Q_INVOKABLE QVariant EditConnectionsModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        return QVariant::Invalid;
    }

    // 行数
    const uint64_t nRow = static_cast<uint64_t>(index.row());

    // 防止越界
    if (nRow > m_vecConnections.size())
    {
        return QVariant::Invalid;
    }
    else if (nRow == m_vecConnections.size())
    {
        // 编辑的那一行
        if (role == Qt::DisplayRole || role == Qt::EditRole)
        {
            return "";
        }

        return QVariant::Invalid;
    }

    // 获取数据
    auto& xData = m_vecConnections[nRow];

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch (index.column())
        {
        case 0:
            return xData.strName;
        case 1:
            return xData.strHosts;
        default:
            break;
        }
    }

    return QVariant::Invalid;

}

Q_INVOKABLE bool EditConnectionsModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid())
    {
        return false;
    }

    if (role == Qt::EditRole)
    {
        QString strNewValue = value.toString();

        size_t nRow = static_cast<size_t>(index.row());

        size_t nColunm = static_cast<size_t>(index.column());

        if (nColunm >= m_vecHeaders.size())
        {
            return false;
        }

        if (nRow > m_vecConnections.size())
        {
            return false;
        }

        if (nRow == m_vecConnections.size()) // 增加数据
        {
            if (strNewValue.isEmpty())
            {
                return false;
            }

            ZKConnection xInfo;

            if (m_vecConnections.rbegin() != m_vecConnections.rend())
            {
                xInfo = *m_vecConnections.rbegin();
            }

            switch (nColunm)
            {
            case 0:
                xInfo.strName = strNewValue;
                break;
            case 1:
                xInfo.strHosts = strNewValue;
                break;
            default:
                break;
            }

            m_vecConnections.push_back(xInfo);

            emit layoutChanged();
        }
        else // 修改现有数据
        {
            auto& xInfo = m_vecConnections[nRow];

            switch (nColunm)
            {
            case 0:
                xInfo.strName = strNewValue;
                break;
            case 1:
                xInfo.strHosts = strNewValue;
                break;
            default:
                break;
            }
        }

        SaveConnectionsToLocalConfig();

        return true;
    }

    return false;
}

Q_INVOKABLE QVariant EditConnectionsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        // 列头
        if (orientation == Qt::Orientation::Vertical)
        {
            if (section < m_vecConnections.size())
            {
                return section + 1;
            }
            else
            {
                return "*";
            }
        }
        // 行头
        else if (orientation == Qt::Orientation::Horizontal)
        {
            if (section >= 0 && static_cast<uint64_t>(section) < m_vecHeaders.size())
            {
                return m_vecHeaders[static_cast<uint64_t>(section)];
            }
        }
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

void EditConnectionsModel::RemoveItems(const std::vector<size_t>& vecRows)
{
    if (vecRows.empty())
    {
        return;
    }

    // 对这些行去重
    std::set<size_t> setRows;
    setRows.insert(vecRows.begin(), vecRows.end());

    std::vector<size_t> vecSimpleRows;
    for (auto it = setRows.begin(); it != setRows.end(); it++)
    {
        if (*it < m_vecConnections.size())
        {
            vecSimpleRows.push_back(*it);
        }

    }

    // 逆序排列
    std::sort(vecSimpleRows.begin(), vecSimpleRows.end(), [](const size_t xFirst, const size_t xSecond)->bool { return xSecond < xFirst; });

    // 从后向前删除
    for (auto& nRow : vecSimpleRows)
    {
        m_vecConnections.erase(m_vecConnections.begin() + nRow);
    }

    emit layoutChanged();
    SaveConnectionsToLocalConfig();
}

bool EditConnectionsModel::GetConnectionInfoByRow(const int nRow, ZKConnection& xOutConInfo)
{
    if (nRow >= 0 && nRow < m_vecConnections.size())
    {
        xOutConInfo = m_vecConnections[nRow];
        return true;
    }

    return false;
}

void EditConnectionsModel::InitConnectionsFromLocalConfig()
{
    std::vector<ZKConnection> vecTemp;

    // 读文件
    QFile xJsonFile(GetConnectionConfigFilePath());

    if (!xJsonFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(nullptr, tr("Info"), tr("Can not find configuration file in Path: ") + GetConnectionConfigFilePath());
        return;
    }

    QByteArray xArray = xJsonFile.readAll();

    // 解析为Json
    QJsonParseError xJsonParseError;
    QJsonDocument xDoc = QJsonDocument::fromJson(xArray, &xJsonParseError);
    if (xDoc.isNull() || !xDoc.isArray())
    {
        QMessageBox::warning(nullptr, tr("Warn"), tr("Failed to convert to QJsonDocument using data from file: ") + GetConnectionConfigFilePath()
                             + ".\n" + tr("Error: ") + xJsonParseError.errorString());
        return;
    }
    const auto& xJsonConnections = xDoc.array();
    for (auto& xOneJsonConnection : xJsonConnections)
    {
        if (!xOneJsonConnection.isObject())
        {
            QMessageBox::warning(nullptr, tr("Warn"), tr("Expect Object. File: ") + GetConnectionConfigFilePath());
            return;
        }

        ZKConnection xCon;

        const auto& xJsonConnectionObject = xOneJsonConnection.toObject();
        auto it = xJsonConnectionObject.find("name");
        if (it == xJsonConnectionObject.constEnd())
        {
            QMessageBox::warning(nullptr, tr("Warn"), tr("Can not find field 'name'. File: ") + GetConnectionConfigFilePath());
            return;
        }
        if (!it->isString())
        {
            QMessageBox::warning(nullptr, tr("Warn"), tr("Expect String. File: ") + GetConnectionConfigFilePath());
            return;
        }
        xCon.strName = it->toString();

        it = xJsonConnectionObject.find("hosts");
        if (it == xJsonConnectionObject.constEnd())
        {
            QMessageBox::warning(nullptr, tr("Warn"), tr("Can not find field 'hosts'. File: ") + GetConnectionConfigFilePath());
            return;
        }
        if (!it->isString())
        {
            QMessageBox::warning(nullptr, tr("Warn"), tr("Expect String. File: ") + GetConnectionConfigFilePath());
            return;
        }
        xCon.strHosts = it->toString();

        vecTemp.push_back(xCon);
    }

    vecTemp.swap(m_vecConnections);
}

void EditConnectionsModel::SaveConnectionsToLocalConfig()
{
    QJsonArray xJsonConnections;

    for (auto& xCon : m_vecConnections)
    {
        QJsonObject xJsonOne;
        xJsonOne["name"] = xCon.strName;
        xJsonOne["hosts"] = xCon.strHosts;

        xJsonConnections.push_back(xJsonOne);
    }

    QJsonDocument xDoc(xJsonConnections);

    const auto& xByteArray = xDoc.toJson(QJsonDocument::Indented);

    QFile xJsonFile(GetConnectionConfigFilePath());

    if (!xJsonFile.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(nullptr, tr("Warn"), tr("Failed to open file: ") + GetConnectionConfigFilePath());
        return;
    }

    xJsonFile.write(xByteArray);
}
