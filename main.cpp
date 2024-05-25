#include "widget.h"
#include "about.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	About about;
	about.show();
	about.exec();

	Widget w;
	w.show();
	return a.exec();
}
