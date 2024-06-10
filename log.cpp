#include "log.h"
#include "ui_log.h"

#include <QDir>
#include <QMessageBox>

#include "architect.h"
#include "process.h"
#include "gtrace.h"
#include "zygote.h"

Log::Log(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::Log)
{
	ui->setupUi(this);
	ui->pbOk->setEnabled(false);
}

Log::~Log() {
	GTRACE("");
	delete ui;
}

void Log::update() {
	Architect::Type type = Architect::getType();
	GTRACE("type=%d", int(type));
	if (type == Architect::TypeNone) {
		QMessageBox mb;
		mb.warning(this, "Error", "Fail to figure out architecture");
		return;
	}

	QString zipFileName = Architect::getFileName(type);
	GTRACE("%s", qPrintable(zipFileName));
	if (zipFileName == "") {
		QMessageBox mb;
		mb.warning(this, "Error", "Filename is empty");
		return;
	}

	ui->pteLog->insertPlainText(QString("Download %1\n\n").arg(zipFileName));
	runProcess("sleep", {"1"});

	{
		QString command	= "curl";
		QStringList arguments{"-O", "-L", QString("https://github.com/jungjin0003/Run-Application-as-Root-for-Android/releases/latest/download/%1").arg(zipFileName)};
		runProcess(command, arguments, ui->pteLog);
		runProcess("sleep", {"1"});
	}

	{
		QString command = "unzip";
		QStringList arguments{"-o", zipFileName};
		runProcess(command, arguments, ui->pteLog);
		runProcess("sleep", {"1"});
	}

	{
		runProcess("chmod", {"766", "injector"}, ui->pteLog);
		runProcess("chmod", {"644", "libhookzygote.so"}, ui->pteLog);
		runProcess("chmod", {"644", "libhookzygote32.so"}, ui->pteLog);
	}

#ifdef Q_OS_ANDROID
	runProcess("su", {"-c", "mount -o rw,remount /system"}, ui->pteLog);

	QString path = QDir::currentPath();
	GTRACE("%s", qPrintable(path));
	switch (type) {
		case Architect::TypeNone:
			GTRACE("Architect::TypeNone");
			break;
		case Architect::Type64:
			runProcess("su", {"-c", QString("cp %1/libhookzygote.so /system/lib64/").arg(path)}, ui->pteLog);
			runProcess("su", {"-c", QString("cp %1/libhookzygote32.so /system/lib/libhookzygote.so").arg(path)}, ui->pteLog);
			break;
		case Architect::Type32:
			runProcess("su", {"-c", QString("cp %1/libhookzygote.so /system/lib/").arg(path)}, ui->pteLog);
	}

	runProcess("su", {"-c", "mount -o ro,remount /system"}, ui->pteLog);
#endif // Q_OS_ANDROID

	ui->pbOk->setEnabled(true);
}

void Log::load() {
	ui->pteLog->insertPlainText("Loading...\n\n");
	runProcess("sleep", {"1"});

#ifdef Q_OS_ANDROID
	runProcess("su", {"-c", "setenforce 0"}, ui->pteLog);
	runProcess("su", {"-c", "getenforce"}, ui->pteLog);
	runProcess("sleep", {"1"});

	QString path = QDir::currentPath();
	QString injectorFilePath = path + "/injector";
	GTRACE("%s", qPrintable(injectorFilePath));
	runProcess("su", {"-c", injectorFilePath}, ui->pteLog);
#endif // Q_OS_ANDROID

	ui->pbOk->setEnabled(true);
}

void Log::unload() {
	ui->pteLog->insertPlainText("Unloading...\n\n");
	runProcess("sleep", {"1"});

#ifdef Q_OS_ANDROID
	int pid;
	Zygote::State state = Zygote::getState(&pid);
	if (state == Zygote::Hooked && pid != 0) {
		QString command = QString("su -c 'kill %1'").arg(pid);
		GTRACE("command=%s", qPrintable(command));
		runProcess(command, {}, ui->pteLog);
	}
#endif // Q_OS_ANDROID

	ui->pbOk->setEnabled(true);
}

void Log::on_pbOk_clicked()
{
	close();
}

