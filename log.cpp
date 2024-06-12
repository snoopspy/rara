#include "log.h"
#include "ui_log.h"

#include <QDir>
#include <QMessageBox>

#include "arch.h"
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
	Arch::Type type = Arch::getType();
	GTRACE("type=%d", int(type));
	if (type == Arch::TypeNone) {
		QMessageBox mb;
		mb.warning(this, "Error", "Fail to figure out architecture");
		return;
	}

	QString zipFileName = Arch::getFileName(type);
	GTRACE("%s", qPrintable(zipFileName));
	if (zipFileName == "") {
		QMessageBox mb;
		mb.warning(this, "Error", "Filename is empty");
		return;
	}

	ui->pteLog->insertPlainText(QString("Download %1\n\n").arg(zipFileName));
	Process::run("sleep", {"1"});

	while (true) {
		Process::PlainTextEditOutput outputPte(ui->pteLog);
		{
			QString command	= "curl";
			QStringList arguments{"-O", "-L", QString("https://github.com/jungjin0003/Run-Application-as-Root-for-Android/releases/latest/download/%1").arg(zipFileName)};
			if (Process::run(command, arguments, &outputPte) != EXIT_SUCCESS) break;
			Process::run("sleep", {"1"});
		}

		{
			QString command = "unzip";
			QStringList arguments{"-o", zipFileName};
			if (Process::run(command, arguments, &outputPte) != EXIT_SUCCESS) break;
			Process::run("sleep", {"1"});
		}

		{
			if (Process::run("chmod", {"777", "injector"}, &outputPte) != EXIT_SUCCESS) break;
			if (Process::run("chmod", {"644", "libhookzygote.so"}, &outputPte) != EXIT_SUCCESS) break;
			if (type == Arch::Type64)
				if (Process::run("chmod", {"644", "libhookzygote32.so"}, &outputPte) != EXIT_SUCCESS) break;
		}

#ifdef Q_OS_ANDROID
		if (!Process::run("su", {"-c", "mount -o rw,remount /system"}, &outputPte) != EXIT_SUCCESS) break;

		QString path = QDir::currentPath();
		GTRACE("%s", qPrintable(path));
		bool ok = true;
		switch (type) {
			case Arch::TypeNone:
				GTRACE("TypeNone");
				break;
			case Arch::Type64:
				if (Process::run("su", {"-c", QString("cp %1/libhookzygote.so /system/lib64/").arg(path)}, &outputPte) != EXIT_SUCCESS) {
					ok = false;
					break;
				}
				if (Process::run("su", {"-c", QString("cp %1/libhookzygote32.so /system/lib/libhookzygote.so").arg(path)}, &outputPte) != EXIT_SUCCESS) {
					ok = false;
					break;
				}
				break;
			case Arch::Type32:
				if (Process::run("su", {"-c", QString("cp %1/libhookzygote.so /system/lib/").arg(path)}, &outputPte) != EXIT_SUCCESS)
					ok = false;
				break;
		}
		if (!ok) break;

		Process::run("su", {"-c", "mount -o ro,remount /system"}, &outputPte);
#endif // Q_OS_ANDROID
		break;
	}

	ui->pbOk->setEnabled(true);
}

void Log::load() {
	ui->pteLog->insertPlainText("Loading...\n\n");
	Process::run("sleep", {"1"});

#ifdef Q_OS_ANDROID
	Process::PlainTextEditOutput outputPte(ui->pteLog);

	Process::run("su", {"-c", "setenforce 0"}, &outputPte);
	Process::run("su", {"-c", "getenforce"}, &outputPte);
	Process::run("sleep", {"1"});

	QString path = QDir::currentPath();
	QString injectorFilePath = path + "/injector";
	GTRACE("%s", qPrintable(injectorFilePath));
	Process::run("su", {"-c", injectorFilePath}, &outputPte);
#endif // Q_OS_ANDROID

	ui->pbOk->setEnabled(true);
}

void Log::unload() {
	ui->pteLog->insertPlainText("Unloading...\n\n");
	Process::run("sleep", {"1"});

#ifdef Q_OS_ANDROID
	int pid;
	Zygote::State state = Zygote::getState(&pid);
	if (state == Zygote::Hooked && pid != 0) {
	Process::PlainTextEditOutput outputPte(ui->pteLog);
		Process::run("su", {"-c", QString("kill %1").arg(pid)}, &outputPte);
	}
#endif // Q_OS_ANDROID

	ui->pbOk->setEnabled(true);
}

void Log::on_pbOk_clicked()
{
	close();
}

