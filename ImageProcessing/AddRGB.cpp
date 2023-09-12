#include "AddRGB.h"

AddRGB::AddRGB(QWidget* parent): QDialog(parent)
{
	ui.setupUi(this);
	setWindowTitle("Load RGB image");

	connect(ui.load_button, SIGNAL(released()), this, SLOT(okButton()));
	connect(ui.cancel_button, SIGNAL(released()), this, SLOT(caButton()));

}

AddRGB::~AddRGB()
{

}

bool AddRGB::initialize(DataManage* the,int* info)
{
	theinfo = info;
	tmanager = the;
	if (tmanager->size() == 0) { return false; }

	Image* img;
	for (int i = 0; i < tmanager->size(); ++i)
	{
		img = tmanager->getimg(i);
		ui.img_box->addItem(QString::fromStdString(img->getName()));
	}

	ui.img_box->setCurrentIndex(0);
	img = tmanager->getimg(0);
	for (int i = 0; i < img->getBand(); ++i)
	{
		ui.r_box->addItem(QString("Band") + QString::number(i + 1));
		ui.g_box->addItem(QString("Band") + QString::number(i + 1));
		ui.b_box->addItem(QString("Band") + QString::number(i + 1));
	}

	connect(ui.img_box, SIGNAL(currentIndexChanged(int)), this, SLOT(setbandnum(int)));
	return true;
}

bool AddRGB::okButton()
{
	if (tmanager->size() == 0)
	{
		QMessageBox::critical(NULL, "Error", "No image has been loaded!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		this->close();
	}

	
	theinfo[0] = ui.img_box->currentIndex();
	theinfo[1] = ui.r_box->currentIndex();
	theinfo[2] = ui.g_box->currentIndex();
	theinfo[3] = ui.b_box->currentIndex();
	
	this->close();
	return true;
}

bool AddRGB::caButton()
{
	this->close();
	theinfo[0] = -1;
	return true;
}

bool AddRGB::setbandnum(int the)
{
	ui.r_box->clear();
	ui.g_box->clear();
	ui.b_box->clear();

	Image* img = tmanager->getimg(the);

	for (int i = 0; i < img->getBand(); ++i)
	{
		ui.r_box->addItem(QString("Band") + QString::number(i + 1));
		ui.g_box->addItem(QString("Band") + QString::number(i + 1));
		ui.b_box->addItem(QString("Band") + QString::number(i + 1));
	}

	return true;
}
