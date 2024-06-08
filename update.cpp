#include "update.h"
#include "ui_update.h"

#include <QMessageBox>
#include <QProcess>
#include <QThread>

#include "architect.h"
#include "gtrace.h"

Update::Update(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::Update)
{
	ui->setupUi(this);

	Architect::Type type = Architect::getType();
	GTRACE("type=%d", int(type));
	if (type == Architect::TypeNone) {
		QMessageBox mb;
		mb.warning(this, "Error", "Fail to figure out architecture");
		return;
	}

	zipFileName_ = Architect::getFileName(type);
	GTRACE("%s", qPrintable(zipFileName_));
	if (zipFileName_ == "") {
		QMessageBox mb;
		mb.warning(this, "Error", "Filename is empty");
		return;
	}

	ui->pteMessage->insertPlainText(QString("Download %1\n\n").arg(zipFileName_));
	ui->pbOk->setEnabled(false);

	QObject::connect(&tmDownload_, &QTimer::timeout, this, &Update::download);
	tmDownload_.start(1000);
}

Update::~Update()
{
	delete ui;
}

void Update::download() {
	tmDownload_.stop();

	QString command	= "curl";
	QStringList arguments{"-O", "-L", QString("https://github.com/jungjin0003/Run-Application-as-Root-for-Android/releases/latest/download//%1").arg(zipFileName_)};
	QProcess process;
	process.start(command, arguments);
	if (!process.waitForStarted()) {
		GTRACE("process.waitForStarted return false");
		return;
	}
	if (!process.waitForFinished()) {
		GTRACE("process.waitForStarted return false");
		return;
	}

	QByteArray ba = process.readAllStandardOutput();
	ui->pteMessage->insertPlainText(ba + "\n");

	ba = process.readAllStandardError();
	ui->pteMessage->insertPlainText(ba + "\n");

	QObject::connect(&tmExtract_, &QTimer::timeout, this, &Update::extract);
	tmExtract_.start(1000);
}

void Update::extract() {
	tmExtract_.stop();

	QString command = "unzip";
	QStringList arguments{"-o", zipFileName_};
	QProcess process;
	process.start(command, arguments);

	if (!process.waitForStarted()) {
		GTRACE("process.waitForStarted return false");
		return;
	}
	if (!process.waitForFinished()) {
		GTRACE("process.waitForStarted return false");
		return;
	}

	QByteArray ba = process.readAllStandardOutput();
	ui->pteMessage->insertPlainText(ba + "\n");

	ba = process.readAllStandardError();
	ui->pteMessage->insertPlainText(ba + "\n");

	system("chmod +x injector");
	system("chmod +x libhookzygote.so");
	system("chmod +x libhookzygote32.so");

	ui->pbOk->setEnabled(true);
	GTRACE("end of update");
}

void Update::on_pbOk_clicked()
{
	close();
}

