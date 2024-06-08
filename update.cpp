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
	ui->pteMessage->insertPlainText("Downloading...\n");
	QObject::connect(&timer_, &QTimer::timeout, this, &Update::download);
	timer_.start(1000);
	ui->pbOk->setEnabled(false);
}

Update::~Update()
{
	delete ui;
}

void Update::download() {
	timer_.stop();
	Architect::Type type = Architect::getType();
	GTRACE("type=%d", int(type));
	if (type == Architect::TypeNone) {
		QMessageBox mb;
		mb.warning(this, "Error", "Fail to figure out architecture");
		return;
	}

	QString fileName = Architect::getFileName(type);
	GTRACE("%s", qPrintable(fileName));
	if (fileName == "") {
		QMessageBox mb;
		mb.warning(this, "Error", "Filename is empty");
		return;
	}

	{
		QString command	= "curl";
		QStringList arguments{"-O", "-L", QString("https://github.com/jungjin0003/Run-Application-as-Root-for-Android/releases/latest/download//%1").arg(fileName)};
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
		ui->pteMessage->insertPlainText(ba);

		ba = process.readAllStandardError();
		ui->pteMessage->insertPlainText(ba);
	}

	ui->pteMessage->repaint();
	QThread::sleep(1);

	{
		QString command = "unzip";
		QStringList arguments{"-o", fileName};
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
		ui->pteMessage->insertPlainText(ba);

		ba = process.readAllStandardError();
		ui->pteMessage->insertPlainText(ba);

		system("chmod +x injector");
		system("chmod +x libhookzygote.so");
		system("chmod +x libhookzygote32.so");
	}

	ui->pbOk->setEnabled(true);
	GTRACE("end of update");
}

void Update::on_pbOk_clicked()
{
	close();
}

