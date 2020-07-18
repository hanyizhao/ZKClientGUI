#include "Language.h"
#include <QFile>
#include <QCoreApplication>

// 获取连接配置文件位置
static QString GetLanguageFilePath()
{
	return QCoreApplication::applicationDirPath() + "/language.txt";
}

int Language::GetNowLanguageIndexFromFile()
{
	// 读文件
	QFile xJsonFile(GetLanguageFilePath());

	if (!xJsonFile.open(QIODevice::ReadOnly))
	{
		return -1;
	}

	QByteArray xArray = xJsonFile.readAll();

	QString strValue(xArray);
	
	return strValue.toInt();
}

void Language::SaveLanguageToFile(const int nIndex)
{
	QFile xJsonFile(GetLanguageFilePath());

	if (!xJsonFile.open(QIODevice::WriteOnly))
	{
		return;
	}
	
	xJsonFile.write(QString::number(nIndex).toUtf8());

	return;
}

QString Language::GetLanguageQMFilePath(const int nIndex)
{
	if (nIndex == 1)
	{
		return ":/translation/zkclientgui_zh-cn.qm";
	}

	return QString();
}
