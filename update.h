#ifndef UPDATE_H
#define UPDATE_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class Update;
}

class Update : public QDialog
{
	Q_OBJECT

public:
	explicit Update(QWidget *parent = nullptr);
	~Update();

	QTimer timer_;
	void download();

private slots:
	void on_pbOk_clicked();

private:
	Ui::Update *ui;
};

#endif // UPDATE_H
