#include "widget.h"
#include "ui_widget.h"

#include <errno.h>
#include <string.h>

#include <QMessageBox>

#include "gtrace.h"

Widget::Widget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::Widget)
{
	ui->setupUi(this);

	ui->tableWidget->setColumnCount(ColumnCount);
	ui->tableWidget->setHorizontalHeaderItem(ColumnPackage, new QTableWidgetItem("Package"));
	ui->tableWidget->setHorizontalHeaderItem(ColumnCheck, new QTableWidgetItem(""));
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(ColumnPackage, QHeaderView::Stretch);

	loadPackageList();
}

Widget::~Widget()
{
	delete ui;
}

void Widget::loadPackageList() {
	QString command("su -c 'cat /data/system/packages.list'");
	FILE* fp = popen(qPrintable(command), "r");
	GTRACE("fp=%p", fp);
	if (fp == nullptr) {
		QMessageBox mb;
		QString msg = QString("Can not open %1 %2").arg(command).arg(QString(strerror(errno)));
		mb.warning(this, "Error", msg);
		return;
	}

	static const int BufSize = 8192;
	while (true) {
		char buf[BufSize];
		char* res = std::fgets(buf, BufSize, fp);
		if (res == nullptr) break;
		GTRACE("res %s", res);

		int row = ui->tableWidget->rowCount();
		ui->tableWidget->insertRow(row);
		GTRACE("row=%d", row);

		ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString(buf)));
	}
	fclose(fp);
}

void Widget::on_pbUpdate_clicked()
{

}


void Widget::on_pbLaunch_clicked()
{

}


void Widget::on_pbReset_clicked()
{

}

