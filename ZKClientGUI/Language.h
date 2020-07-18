#pragma once

#include <QString>

class Language
{
public:
	// If can not find config. return -1.
	static int GetNowLanguageIndexFromFile();
	static void SaveLanguageToFile(const int nIndex);

	static QString GetLanguageQMFilePath(const int nIndex);

};