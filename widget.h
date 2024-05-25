#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QList>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

struct Package {
	Package(QString name, bool checked = false) : name_(name), checked_(checked) {}
	QString name_;
	bool checked_;
};

struct PackageList : QList<Package> {
};

class Widget : public QWidget
{
	Q_OBJECT

public:
	Widget(QWidget *parent = nullptr);
	~Widget();

	PackageList packageList_;
	void loadPackageList();
	void showPackageList(QString filter = "");

public:
	const static int ColumnPackage = 0;
	const static int ColumnCheck = 1;
	const static int ColumnCount = 2;

private slots:

	void on_leFilter_returnPressed();

	void on_tbFilter_clicked();

	void on_pbUpdate_clicked();

	void on_pbLoad_clicked();

	void on_pbUnload_clicked();

private:
	Ui::Widget *ui;
};
#endif // WIDGET_H
