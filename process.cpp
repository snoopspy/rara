#include "process.h"

#include <QEventLoop>

QString runProcess(QString command, QStringList arguments, QPlainTextEdit* plainTextEdit, QString input)  {
	if (plainTextEdit != nullptr) {
		plainTextEdit->insertPlainText("\n" + command + " " + arguments.join(' ') + "\n");
	}
	QString result;
	QEventLoop looper;
	QProcess* p = new QProcess(&looper);
	p->setProcessChannelMode(QProcess::MergedChannels);

	QObject::connect(p, &QProcess::finished, &looper, &QEventLoop::quit);

	QObject::connect(p, &QProcess::errorOccurred, [&result]()->void{
		qDebug("Error in Process");
		result.clear();
	});

	QObject::connect(p, &QProcess::errorOccurred, &looper,&QEventLoop::quit);

	QObject::connect(p, &QProcess::started, [p,&input]()->void{
		p->write(qPrintable(input +'\n'));
	});

	QObject::connect(p, &QProcess::readyReadStandardOutput, [p, plainTextEdit, &result]()->void{
		QByteArray ba = p->readAllStandardOutput();
		qDebug() << ba;
		if (plainTextEdit != nullptr) {
			plainTextEdit->insertPlainText(ba);
		}
		result+= ba;
	});

	QObject::connect(p, &QProcess::readyReadStandardError, [p, plainTextEdit]()->void{
		QByteArray ba = p->readAllStandardError();
		qDebug() << ba;
		if (plainTextEdit != nullptr) {
			plainTextEdit->insertPlainText(ba);
		}
	});

	p->start(command, arguments);
	looper.exec();
	return result;
}
