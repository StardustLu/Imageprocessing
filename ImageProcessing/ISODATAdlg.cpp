#include "ISODATAdlg.h"

ISODATAdlg::ISODATAdlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

ISODATAdlg::~ISODATAdlg()
{
}

bool ISODATAdlg::initialize(DataManage* td,int* tindex,bool* tdoit, string* opath, int* class_num, int* max_it, int* minnum, int* max_std, int* mindis)
{
	manager = td;
	doit = tdoit;
	outpath = opath;
	classnum = class_num;
	max_iter = max_it;
	min_num = minnum;
	max_stdv = max_std;
	min_dis = mindis;
	theindex = tindex;

	ui.max_iter->setText("10");
	ui.classline->setText("3");
	ui.min_count->setText("500");
	ui.max_std->setText("20");
	ui.min_dis->setText("100");

	ui.outpathline->setText("F:/test/RSProcessing/result/isodata.tif");
	
	Image* img;
	for (int i = 0; i < manager->size(); ++i)
	{
		img = manager->getimg(i);
		ui.comboBox->addItem(QString::fromStdString(img->getName()));
	}

	connect(ui.okbutton, SIGNAL(released()), this, SLOT(okisoButton()));
	connect(ui.cancelbutton, SIGNAL(released()), this, SLOT(caisoButton()));
	connect(ui.pathbutton, SIGNAL(released()), this, SLOT(fileisoButton()));

	return true;
}

bool ISODATAdlg::caisoButton()
{
	this->close();
	return true;
}

bool ISODATAdlg::fileisoButton()
{
	QFileDialog filedlg;
	QString fpath;
	fpath = filedlg.getSaveFileName(this, tr("Open Image"), ".", tr("Image Files(*.tif)"));

	(*outpath) = fpath.toStdString();
	ui.outpathline->setText(fpath);

	return true;
}

bool ISODATAdlg::okisoButton()
{
	(*doit) = true;

	QString thepat = ui.outpathline->text();
	(*outpath) = thepat.toStdString();
	QString temp;

	temp = ui.classline->text();
	(*classnum) = temp.toInt();

	temp= ui.max_iter->text();
	(*max_iter) = temp.toInt();

	temp = ui.min_count->text();
	(*min_num) = temp.toInt();

	temp = ui.max_std->text();
	(*max_stdv) = temp.toInt();

	temp = ui.min_dis->text();
	(*min_dis) = temp.toInt();

	(*theindex) = ui.comboBox->currentIndex();

	this->close();
	return true;
}