#include <QApplication>
#include "about.h"
#include "gsignal.h"
#include "gtrace.h"
#include "option.h"
#include "widget.h"

struct Signal : QObject {
	Q_OBJECT

public:
	Signal() {
		GSignal& signal = GSignal::instance();
		QObject::connect(&signal, &GSignal::signaled, this, &Signal::processSignal, Qt::DirectConnection);
		signal.setupAll();
	}

public slots:
	void processSignal(int signo) {
#ifdef Q_OS_WIN
		(void)signo;
#else // Q_OS_WIN
	QString str1 = GSignal::getString(signo);
	QString str2 = strsignal(signo);
	GTRACE("signo=%d signal=%s msg=%s", signo, qPrintable(str1), qPrintable(str2));
	QApplication::quit();
#endif
	}
};

int main(int argc, char *argv[]) {
	gtrace_default("127.0.0.1", 8908, true, "rara.log");
	gtrace("");
	gtrace("rara %s",
#include "version.txt"
	);

	QApplication a(argc, argv);
	Signal signal;
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

#include "rara.moc"
