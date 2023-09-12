#include "Otsudlg.h"

Otsudlg::Otsudlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

Otsudlg::~Otsudlg()
{

}

bool Otsudlg::initialize(DataManage* tm, bool* do_it, int* imgnum,int* theband, string* opath,QString& name)
{
	manager = tm;
	doit = do_it;
	num = imgnum;
	path = opath;
	tband = theband;

	setWindowTitle(name);
	ui.lineEdit->setText(QString("F:/test/RSProcessing/result/seg.tif"));

	Image* img;
	for (int i = 0; i < manager->size(); ++i)
	{
		img = manager->getimg(i);
		ui.comboBox->addItem(QString::fromStdString(img->getName()));
	}

	for (int i = 0; i < img->getBand(); ++i)
	{
		ui.bandcombo->addItem(QString("Band") + QString::number(i + 1));
	}

	connect(ui.okbutton, SIGNAL(released()), this, SLOT(okOtsuButton()));
	connect(ui.canbutton, SIGNAL(released()), this, SLOT(caOtsuButton()));
	connect(ui.filebutton, SIGNAL(released()), this, SLOT(fileOtsuButton()));
	connect(ui.bandcombo, SIGNAL(currentIndexChanged(int)), this, SLOT(setband(int)));

	return true;
}


bool Otsudlg::okOtsuButton()
{
	(*doit) = true;

	(*num) = ui.comboBox->currentIndex();
	(*tband) = ui.bandcombo->currentIndex();
	QString fpath = ui.lineEdit->text();
	(*path) = fpath.toStdString();
	this->close();
	return true;
}

bool Otsudlg::caOtsuButton()
{
	this->close();
	return false;
}

bool Otsudlg::fileOtsuButton()
{
	QFileDialog filedlg;
	QString fpath;
	fpath = filedlg.getSaveFileName(this, tr("Open Image"), ".", tr("Image Files(*.tif)"));

	(*path) = fpath.toStdString();
	ui.lineEdit->setText(fpath);

	return true;
}

bool Otsudlg::setband(int the)
{
	ui.bandcombo->clear();
	Image* img = manager->getimg(the);

	ui.bandcombo->addItem(QString("All Band"));
	for (int i = 0; i < img->getBand(); ++i)
	{
		ui.bandcombo->addItem(QString("Band") + QString::number(i + 1));
	}

	return true;
}
