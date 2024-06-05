#include "widget.h"
#include "about.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	About about;
#ifdef Q_OS_ANDROID
	about.showMaximized();
#else
	about.show();
#endif // Q_OS_ANDROID
	about.exec();

	Widget w;
	w.show();
	return a.exec();
}
