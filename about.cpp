#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::About)
{
	ui->setupUi(this);

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
