#include "widget.h"
#include "ui_widget.h"

#include <QFile>
#include "gcheckbox.h"
#include "log.h"

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

	setControl();
}

Widget::~Widget() {
	GTRACE("");
	delete ui;
}

void Widget::setControl() {
	bool injectorExists = QFile::exists("injector");
	Zygote::State state = Zygote::getState();
	GTRACE("exist=%d state=%d", int(injectorExists), int(state));

	ui->pbUpdate->setEnabled(true);
	ui->pbLoad->setEnabled(injectorExists && state == Zygote::Unhooked);
	ui->pbUnload->setEnabled(state != Zygote::Hooked);
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
	Log log(this);
#ifdef Q_OS_ANDROID
	log.showMaximized();
#else
	log.show();
#endif // Q_OS_ANDROID
	log.update();
	log.exec();
	setControl();
}

void Widget::on_pbLoad_clicked() {
	Log log(this);
#ifdef Q_OS_ANDROID
	log.showMaximized();
#else
	log.show();
#endif // Q_OS_ANDROID
	log.load();
	log.exec();
	setControl();
}

void Widget::on_pbUnload_clicked() {
	Log log(this);
#ifdef Q_OS_ANDROID
	log.showMaximized();
#else
	log.show();
#endif // Q_OS_ANDROID
	log.unload();
	log.exec();
	setControl();
}
