#include "update.h"
#include "ui_update.h"

#include <QDir>
#include <QMessageBox>

#include "process.h"
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
	runProcess(command, arguments, ui->pteMessage);

	QObject::connect(&tmExtract_, &QTimer::timeout, this, &Update::extract);
	tmExtract_.start(1000);
}

void Update::extract() {
	tmExtract_.stop();

	QString command = "unzip";
	QStringList arguments{"-o", zipFileName_};

	runProcess(command, arguments, ui->pteMessage);

	runProcess("chmod", {"+x", "injector"}, ui->pteMessage);
	runProcess("chmod", {"+x", "libhookzygote.so"}, ui->pteMessage);
	runProcess("chmod", {"+x", "libhookzygote32.so"}, ui->pteMessage);

#ifdef Q_OS_ANDROID
	runProcess("su", {"-c", "mount -o rw,remount /system"}, ui->pteMessage);

	QString path = QDir::currentPath();
	GTRACE("%s", qPrintable(path));
	Architect::Type type = Architect::getType();
	switch (type) {
		case Architect::TypeNone:
			GTRACE("Architect::TypeNone");
			break;
		case Architect::Type64:
			runProcess("su", {"-c", QString("cp %1/libhookzygote.so /system/lib64/").arg(path)}, ui->pteMessage);
			runProcess("su", {"-c", QString("cp %1/libhookzygote32.so /system/lib/libhookzygote.so").arg(path)}, ui->pteMessage);
			break;
		case Architect::Type32:
			runProcess("su", {"-c", QString("cp %1/libhookzygote.so /system/lib/").arg(path)}, ui->pteMessage);
	}

	runProcess("su", {"-c", "mount -o ro,remount /system"}, ui->pteMessage);
#endif // Q_OS_ANDROID

	ui->pbOk->setEnabled(true);
	GTRACE("end of update");
}

void Update::on_pbOk_clicked()
{
	close();
}

