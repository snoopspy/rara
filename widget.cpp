#include "widget.h"
#include "ui_widget.h"

#include <errno.h>
#include <string.h>

#include <QMessageBox>

#include "gtrace.h"

Widget::Widget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::Widget) {
	ui->setupUi(this);

	ui->tableWidget->setColumnCount(ColumnCount);
	ui->tableWidget->setHorizontalHeaderItem(ColumnPackage, new QTableWidgetItem("Package"));
	ui->tableWidget->setHorizontalHeaderItem(ColumnCheck, new QTableWidgetItem(""));
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(ColumnPackage, QHeaderView::Stretch);

	loadPackageList();
	showPackageList("");
}

Widget::~Widget() {
	delete ui;
}

void Widget::loadPackageList() {
	static const int BufSize = 8192;
	QString runAppFileName("runapp.list");
	QSet<QString> runApps;
	FILE* fp = fopen(qPrintable(runAppFileName), "rt");
	if (fp != nullptr) {
		while (true) {
			char buf[BufSize];
			char* res = std::fgets(buf, BufSize, fp);
			if (res == nullptr) break;
			runApps.insert(QString(buf));
		}
		fclose(fp);
	}
	for (QString runApp: runApps)
		GTRACE("runapp : %s", qPrintable(runApp));

#ifdef Q_OS_ANDROID
	QString packageListCommand("su -c 'cat /data/system/packages.list'");
#else // Q_OS_ANDROID
	QString packageListCommand("cat packages.list");
#endif // Q_OS_ANDROID

	fp = popen(qPrintable(packageListCommand), "r");
	GTRACE("fp=%p", fp);
	if (fp == nullptr) {
		QMessageBox mb;
		QString msg = QString("Can not open %1 %2").arg(packageListCommand).arg(QString(strerror(errno)));
		mb.warning(this, "Error", msg);
		return;
	}

	packageList_.clear();
	while (true) {
		char buf[BufSize];
		char packageName[BufSize];
		char* res = std::fgets(buf, BufSize, fp);
		if (res == nullptr) break;
		sscanf(buf, "%s", packageName);
		QSet<QString>::iterator it = runApps.find(QString(packageName));
		packageList_.push_back(Package(packageName, it != runApps.end()));
	}
	pclose(fp);
}

void Widget::showPackageList(QString filter) {
	filter = filter.trimmed();
	ui->tableWidget->setRowCount(0);
	for (Package package: packageList_) {
		if (filter != "" && package.name_.indexOf(filter) == -1) continue;
		int row = ui->tableWidget->rowCount();
		ui->tableWidget->insertRow(row);
		ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString(package.name_)));
	}
}

void Widget::on_leFilter_returnPressed() {
	ui->tbFilter->click();
}

void Widget::on_tbFilter_clicked() {
	showPackageList(ui->leFilter->text());
}

void Widget::on_pbUpdate_clicked() {
	GTRACE("");
}

void Widget::on_pbLoad_clicked() {
	GTRACE("");
}

void Widget::on_pbUnload_clicked() {
	GTRACE("");
}
