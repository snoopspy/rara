#ifndef PROCESS_H
#define PROCESS_H

#include <QProcess>
#include <QPlainTextEdit>
#include <QScrollBar>

struct Process {
	struct Output {
		Output() {}
		virtual void write(QString data) = 0;
	};
	struct PlainTextEditOutput : Output {
		PlainTextEditOutput(QPlainTextEdit* plainTextEdit) : Output(), plainTextEdit_(plainTextEdit) {}
		QPlainTextEdit* plainTextEdit_;
		void write(QString data) override {
			plainTextEdit_->insertPlainText(data);
			plainTextEdit_->verticalScrollBar()->setValue(plainTextEdit_->verticalScrollBar()->maximum());
		};
	};

	static int run(QString command, QStringList arguments, Output* output = nullptr, QString input = QString());
};

#endif // PROCESS_H
