#include "architect.h"

#include <QProcess>
#include "gtrace.h"

Architect::Type Architect::getType() {
	QString program{"uname"};
	QStringList arguments{"-m"};
	QProcess process;
	process.start(program, arguments, QIODeviceBase::ReadOnly);
	if (!process.waitForFinished()) {
		GTRACE("process.waitForFninshed return false");
		return TypeNone;
	}
	QByteArray result = process.readAllStandardOutput();
	GTRACE("result=%s", qPrintable(result));
	if (result.indexOf("64") != -1) return Type64; // x86_64, aarch64
	if (result.indexOf("armv8l") != -1) return Type64; // armv8l
	if (result.indexOf("armv7l") != -1) return Type32; // armv7l
	return TypeNone;
}
