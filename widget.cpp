#include "widget.h"
#include "ui_widget.h"

#include "gcheckbox.h"
#include "update.h"

Widget::Widget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::Widget) {
	ui->setupUi(this);

	setWindowTitle("R.A.R.A");

	ui->tableWidget->verticalHeader()->setVisible(false);
	ui->tableWidget->setColumnCount(ColumnCount);
	ui->tableWidget->setHorizontalHeaderItem(ColumnPackage, new QTableWidgetItem("Package"));
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(ColumnPackage, QHeaderView::Stretch);

	packages_.load();
	showPackages("");
}

Widget::~Widget() {
	delete ui;
}

void Widget::showPackages(QString filter) {
	filter = filter.trimmed();
	ui->tableWidget->setRowCount(0);
	for (Package package: packages_) {
		if (filter != "" && package.name_.indexOf(filter) == -1) continue;
		int row = ui->tableWidget->rowCount();
		ui->tableWidget->insertRow(row);
		//ui->tableWidget->setItem(row, ColumnPackage, new QTableWidgetItem(QString(package.name_)));
		GCheckBox* checkBox = new GCheckBox(ui->tableWidget);
		checkBox->setProperty("packageName", package.name_);

		QObject::connect(checkBox, &GCheckBox::clicked, [this, checkBox]()->void {
			QString packageName = checkBox->property("packageName").toString();
			GTRACE("%s %d", qPrintable(packageName), int(checkBox->isChecked()));
			Package package(packageName, checkBox->isChecked());
			packages_.remove(packageName);
			packages_.insert(packageName, package);
		});
		checkBox->setText(package.name_);
		checkBox->setChecked(package.checked_);
		ui->tableWidget->setCellWidget(row, ColumnPackage, checkBox);
	}
}

void Widget::on_leFilter_returnPressed() {
	ui->tbFilter->click();
}

void Widget::on_tbFilter_clicked() {
	showPackages(ui->leFilter->text());
}

void Widget::on_pbUpdate_clicked() {
	Update update(this);
#ifdef Q_OS_ANDROID
	update.showMaximized();
#else
	update.show();
#endif // Q_OS_ANDROID
	update.exec();
}

void Widget::on_pbLoad_clicked() {
	GTRACE("");
	packages_.save();
}

void Widget::on_pbUnload_clicked() {
	GTRACE("");
	packages_.save();
}
