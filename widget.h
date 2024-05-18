#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

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

	void loadPackageList();

public:
	const static int ColumnPackage = 0;
	const static int ColumnCheck = 1;
	const static int ColumnCount = 2;

private slots:
	void on_pbUpdate_clicked();

	void on_pbLaunch_clicked();

	void on_pbReset_clicked();

private:
	Ui::Widget *ui;
};
#endif // WIDGET_H
