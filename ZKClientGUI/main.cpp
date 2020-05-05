#include "ZKClientGUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ZKClientGUI w;
	w.show();
	return a.exec();
}
