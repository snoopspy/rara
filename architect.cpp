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
	QByteArray ba = process.readAllStandardOutput();
	GTRACE("result=%s", qPrintable(ba));
	if (ba.indexOf("64") != -1) return Type64; // x86_64, aarch64
	if (ba.indexOf("armv8l") != -1) return Type64; // armv8l
	if (ba.indexOf("armv7l") != -1) return Type32; // armv7l
	return TypeNone;
}

QString Architect::getFileName(Type type) {
	switch (type) {
		case TypeNone:
			GTRACE("type is TypeNone");
			return QString();
		case Architect::Type64:
			return QString("Run-Application-as-Root-for-Android_linux_aarch64.zip");
		case Architect::Type32:
			return QString("Run-Application-as-Root-for-Android_linux_armv7a.zip");
	}
}
