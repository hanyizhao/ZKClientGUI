#include "ZKClientGUI.h"
#include <QtWidgets/QApplication>
#include "Language.h"
#include <QTranslator>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    QTranslator xTranslator;

    const auto nLanguageIndex = Language::GetNowLanguageIndexFromFile();
    if (nLanguageIndex >= 0)
    {
        const auto strLanguageFilePath = Language::GetLanguageQMFilePath(nLanguageIndex);
        if (!strLanguageFilePath.isEmpty())
        {
			xTranslator.load(strLanguageFilePath);
			a.installTranslator(&xTranslator);
        }
    }

    ZKClientGUI w(nullptr, nLanguageIndex);
    w.show();
    return a.exec();
}
