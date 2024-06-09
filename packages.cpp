#include "packages.h"
#include <QMessageBox>

bool Packages::load() {
	static const int BufSize = 8192;
	QSet<QString> runApps;
	runApps.clear();
	FILE* fp = fopen(qPrintable(rootAppFileName), "rt");
	if (fp != nullptr) {
		while (true) {
			char buf[BufSize];
			char* res = std::fgets(buf, BufSize, fp);
			if (res == nullptr) break;
			QString packageName(buf);
			if (packageName.endsWith("\n")) packageName = packageName.left(packageName.size() - 1);
			runApps.insert(QString(packageName));
		}
		fclose(fp);
	}

#ifdef Q_OS_ANDROID
	QString packageListCommand("su -c 'cat /data/system/packages.list'");
#else // Q_OS_ANDROID
	QString packageListCommand("cat packages.list");
#endif // Q_OS_ANDROID

	fp = popen(qPrintable(packageListCommand), "r");
	if (fp == nullptr) {
		QMessageBox mb;
		QString msg = QString("Can not open %1 %2").arg(packageListCommand).arg(QString(strerror(errno)));
		mb.warning(nullptr, "Error", msg);
		return false;
	}

	clear();
	while (true) {
		char buf[BufSize];
		char packageName[BufSize];
		memset(packageName, 0, BufSize);
		char* res = std::fgets(buf, BufSize, fp);
		GTRACE("%s", buf);
		if (res == nullptr) break;
		sscanf(buf, "%s", packageName);
		QSet<QString>::iterator it = runApps.find(QString(packageName));
		insert(packageName, Package(QString(packageName), it != runApps.end()));
	}
	pclose(fp);

	return true;
}

bool Packages::save() {
	QString command = QString("rm -f %1").arg(rootAppFileName);
	system(qPrintable(command));

	FILE* fp= fopen(qPrintable(rootAppFileName), "wt");
	for (Packages::iterator it = begin(); it != end(); it++) {
		Package package = it.value();
		if (package.checked_) {
			std::fputs(qPrintable(package.name_ + "\n"), fp);
		}
	}
	fclose(fp);
	return true;
}
