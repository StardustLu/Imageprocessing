#include "Networkdlg.h"

Networkdlg::Networkdlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

Networkdlg::~Networkdlg()
{
}

bool Networkdlg::initialize(DataManage* tm, bool* doit, int* index, string* outpath, int* layers, int* nurons, double* learning_rate, int* max_iter)
{
	manager = tm;
	tdoit = doit;
	theindex = index;
	opath = outpath;
	thelayers = layers;
	thenuuros = nurons;
	thelearnrate = learning_rate;
	themax = max_iter;

	ui.maxiterbox->setMaximum(100000);

	ui.layersbox->setValue(2);
	ui.neuronbox->setValue(30);
	ui.learningratebox->setValue(0.5);
	ui.maxiterbox->setValue(200);

	ui.outpathedit->setText(QString("F:/test/RSProcessing/result/bptest.tif"));

	Image* img;
	for (int i = 0; i < manager->size(); ++i)
	{
		img = manager->getimg(i);
		ui.comboBox->addItem(QString::fromStdString(img->getName()));
	}

	connect(ui.okbutton, SIGNAL(released()), this, SLOT(okNButton()));
	connect(ui.cancelbutton, SIGNAL(released()), this, SLOT(canNButton()));
	connect(ui.filebotton, SIGNAL(released()), this, SLOT(fileNButton()));



	return true;
}

bool Networkdlg::canNButton()
{
	this->close();
	return false;
}

bool Networkdlg::fileNButton()
{
	QFileDialog filedlg;
	QString fpath;
	fpath = filedlg.getSaveFileName(this, tr("Open Image"), ".", tr("Image Files(*.tif)"));

	(*opath) = fpath.toStdString();
	ui.outpathedit->setText(fpath);

	return true;
}

bool Networkdlg::okNButton()
{
	(*tdoit) = true;
	QString thepat = ui.outpathedit->text();
	(*opath) = thepat.toStdString();

	(*theindex) = ui.comboBox->currentIndex();

	(*thelayers) = ui.layersbox->value();
	(*thenuuros) = ui.neuronbox->value();
	(*thelearnrate) = ui.learningratebox->value();
	(*themax) = ui.maxiterbox->value();

	this->close();
	return true;
}