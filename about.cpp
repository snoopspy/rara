#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::About)
{
	ui->setupUi(this);

	setWindowTitle("R.A.R.A");

	ui->lblCrazyHacker->setTextFormat(Qt::RichText);
	ui->lblCrazyHacker->setTextInteractionFlags(Qt::TextBrowserInteraction);
	ui->lblCrazyHacker->setOpenExternalLinks(true);

	ui->lblGilgil->setTextFormat(Qt::RichText);
	ui->lblGilgil->setTextInteractionFlags(Qt::TextBrowserInteraction);
	ui->lblGilgil->setOpenExternalLinks(true);
}

About::~About()
{
	delete ui;
}
