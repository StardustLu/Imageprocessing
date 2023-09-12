#include "SVMdlg.h"

SVMdlg::SVMdlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

SVMdlg::~SVMdlg()
{
}

bool SVMdlg::initialize(DataManage* tm, int* index, bool* thedo, string* thepath,int* thekernl, double* thec, double* thegamma, int* thed)
{
	manager = tm;
	doti = thedo;
	theindex = index;
	opath = thepath;
	c = thec;
	gamma = thegamma;
	d = thed;
	kernel = thekernl;

	Image* img;
	for (int i = 0; i < manager->size(); ++i)
	{
		img = manager->getimg(i);
		ui.imgcombobox->addItem(QString::fromStdString(img->getName()));
	}

	ui.kernelcombobox->addItem(QString("Linear"));
	ui.kernelcombobox->addItem(QString("Polynomial"));
	ui.kernelcombobox->addItem(QString("RBF"));

	ui.cedit->setText("1");
	ui.gammaedit->setText("0.000001");
	ui.dedit->setText("3");

	connect(ui.okbutton, SIGNAL(released()), this, SLOT(oksvmButton()));
	connect(ui.cancelbutton, SIGNAL(released()), this, SLOT(cansvmButton()));
	connect(ui.filebutton, SIGNAL(released()), this, SLOT(filesvmButton()));

	return true;
}

bool SVMdlg::cansvmButton()
{
	this->close();
	return false;
}

bool SVMdlg::filesvmButton()
{
	QFileDialog filedlg;
	QString fpath;
	fpath = filedlg.getSaveFileName(this, tr("Open Image"), ".", tr("Image Files(*.tif)"));

	(*opath) = fpath.toStdString();
	ui.fileedit->setText(fpath);

	return true;
}


bool SVMdlg::oksvmButton()
{
	(*doti) = true;
	(*theindex) = ui.imgcombobox->currentIndex();
	(*kernel) = ui.kernelcombobox->currentIndex();

	QString thepath = ui.fileedit->text();
	(*opath) = thepath.toStdString();

	QString temp;

	temp = ui.cedit->text();
	(*c) = temp.toInt();

	temp = ui.gammaedit->text();
	(*gamma) = temp.toInt();

	temp = ui.dedit->text();
	(*d) = temp.toInt();

	this->close();
	return true;
}