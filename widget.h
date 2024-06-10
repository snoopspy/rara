#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QList>

#include "packages.h"
#include "zygote.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE


class Widget : public QWidget
{
	Q_OBJECT

public:
	Widget(QWidget *parent = nullptr);
	~Widget();

	void setControl();

	Packages packages_;
	void showPackages(QString filter, bool showOnlySelected);

public:
	const static int ColumnPackage = 0;
	const static int ColumnCount = 1;

private slots:

	void on_leFilter_returnPressed();

	void on_tbFilter_clicked();

	void on_pbUpdate_clicked();

	void on_pbLoad_clicked();

	void on_pbUnload_clicked();

	void on_chkShowOnlySelected_clicked();

private:
	Ui::Widget *ui;
};
#endif // WIDGET_H
