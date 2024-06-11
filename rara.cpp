#include <QApplication>
#include "about.h"
#include "gtrace.h"
#include "option.h"
#include "widget.h"

int main(int argc, char *argv[]) {
	gtrace_default("127.0.0.1", 8908, true, "rara.log");
	gtrace("");
	gtrace("rara %s",
#include "version.txt"
	);

	QApplication a(argc, argv);

	Option& option = Option::instance();
	if (option.showDialog_) {
		About about;
#ifdef Q_OS_ANDROID
		about.showMaximized();
#else
		about.show();
#endif // Q_OS_ANDROID
		about.exec();
	}

	Widget* w = new Widget;
	w->show();
	int res = a.exec();
	delete w;
	GTRACE("rara terminated successfully");
	return res;
}
