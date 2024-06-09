#ifndef LOG_H
#define LOG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class Log;
}

class Log : public QDialog
{
	Q_OBJECT

public:
	explicit Log(QWidget *parent = nullptr);
	~Log();

	void update();
	void load();
	void unload();

private slots:
	void on_pbOk_clicked();

private:
	Ui::Log *ui;
};

#endif // LOG_H
