#include "mainwin.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication application(argc, argv);
	MainWin window;

	window.show();

	return application.exec();
}