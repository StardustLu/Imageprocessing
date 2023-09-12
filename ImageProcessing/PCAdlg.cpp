#include "PCAdlg.h"

PCAdlg::PCAdlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

PCAdlg::~PCAdlg()
{
}

bool PCAdlg::initialize(DataManage* tm, bool* do_it, int* imgnum,string* fp,string* ip,QString& thename)
{
	manager = tm;
	doit = do_it;
	num = imgnum;
	forpath = fp;
	invpath = ip;
	setWindowTitle(thename);

	ui.forEdit->setText("F:/test/RSProcessing/result/pcafor.tif");
	ui.invEdit->setText("F:/test/RSProcessing/result/pcainv.tif");

	Image* img;
	for (int i = 0; i < manager->size(); ++i)
	{
		img = manager->getimg(i);
		ui.comboBox->addItem(QString::fromStdString(img->getName()));
	}

	connect(ui.transButton, SIGNAL(released()), this, SLOT(okpcaButton()));
	connect(ui.cancelButton, SIGNAL(released()), this, SLOT(capcaButton()));
	connect(ui.forButton, SIGNAL(released()), this, SLOT(forbut()));
	connect(ui.invButton, SIGNAL(released()), this, SLOT(invbut()));

	return true;
}

bool PCAdlg::okpcaButton()
{
	(*doit) = true;
	(*num) = ui.comboBox->currentIndex();

	QString tforpath = ui.forEdit->text();
	QString finvpath = ui.invEdit->text();

	(*forpath) = tforpath.toStdString();
	(*invpath) = finvpath.toStdString();

	this->close();

	return true;
}

bool PCAdlg::capcaButton()
{
	this->close();
	return false;
}

bool PCAdlg::forbut()
{
	QFileDialog filedlg;
	QString fpath;
	fpath = filedlg.getSaveFileName(this, tr("Open Image"), ".", tr("Image Files(*.tif)"));

	(*forpath) = fpath.toStdString();
	ui.forEdit->setText(fpath);

	return true;
}

bool PCAdlg::invbut()
{
	QFileDialog filedlg;
	QString fpath;
	fpath = filedlg.getSaveFileName(this, tr("Open Image"), ".", tr("Image Files(*.tif)"));

	(*invpath) = fpath.toStdString();
	ui.invEdit->setText(fpath);

	return true;

}
