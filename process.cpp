#include "process.h"

#include <QEventLoop>

int Process::run(QString command, QStringList arguments, Output* output, QString input) {
	if (output != nullptr) output->write("\n" + command + " " + arguments.join(' ') + "\n");

	QEventLoop looper;
	QProcess* p = new QProcess(&looper);
	p->setProcessChannelMode(QProcess::MergedChannels);

	QObject::connect(p, &QProcess::finished, &looper, &QEventLoop::quit);

	QObject::connect(p, &QProcess::errorOccurred, []()->void{
		qDebug("Error in Process");
	});

	QObject::connect(p, &QProcess::errorOccurred, &looper,&QEventLoop::quit);

	QObject::connect(p, &QProcess::started, [p,&input]()->void{
		p->write(qPrintable(input +'\n'));
	});

	QObject::connect(p, &QProcess::readyReadStandardOutput, [p, output]()->void{
		QByteArray ba = p->readAllStandardOutput();
		qDebug() << ba;
		if (output != nullptr) output->write(ba);
	});

	QObject::connect(p, &QProcess::readyReadStandardError, [p, output]()->void{
		QByteArray ba = p->readAllStandardError();
		qDebug() << ba;
		if (output != nullptr) output->write(ba);
	});

	p->start(command, arguments);
	looper.exec();
	return p->exitCode();
}
