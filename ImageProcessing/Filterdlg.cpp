#include "Filterdlg.h"

Filterdlg::Filterdlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

Filterdlg::~Filterdlg()
{
}

bool Filterdlg::initialize(DataManage* tm, bool* doit, int* index,string* outpath, int* kernel_row, int* kernel_col)
{
	manager = tm;
	tdoti = doit;
	tindex = index;
	krow = kernel_row;
	kcol = kernel_col;
	opath = outpath;

	setWindowTitle("Majority");

	Image* img;
	for (int i = 0; i < manager->size(); ++i)
	{
		img = manager->getimg(i);
		ui.comboBox->addItem(QString::fromStdString(img->getName()));
	}

	ui.colbox->setValue(3);
	ui.rowbox->setValue(3);

	connect(ui.okbutton, SIGNAL(released()), this, SLOT(okFButton()));
	connect(ui.cancelbutton, SIGNAL(released()), this, SLOT(caFButton()));
	connect(ui.filebutton, SIGNAL(released()), this, SLOT(fileFButton()));

	return true;
}

bool Filterdlg::caFButton()
{
	this->close();
	return false;
}

bool Filterdlg::fileFButton()
{
	QFileDialog filedlg;
	QString fpath;
	fpath = filedlg.getSaveFileName(this, tr("Open Image"), ".", tr("Image Files(*.tif)"));

	(*opath) = fpath.toStdString();
	ui.outedit->setText(fpath);

	return true;
}

bool Filterdlg::okFButton()
{

	(*tdoti) = true;
	(*tindex) = ui.comboBox->currentIndex();

	(*krow) = ui.rowbox->value();
	(*kcol) = ui.colbox->value();

	QString fpath = ui.outedit->text();
	(*opath) = fpath.toStdString();
	this->close();
	return true;

}
