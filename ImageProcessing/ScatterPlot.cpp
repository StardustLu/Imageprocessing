#include "ScatterPlot.h"

ScatterPlot::ScatterPlot(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setFixedSize(600, 700);

	back_img = QImage(600, 700, QImage::Format_RGB32);
	QColor backColour = qRgb(255, 255, 255);
	back_img.fill(backColour);

	thepainter = new QPainter(&back_img);
	thepainter->setRenderHint(QPainter::Antialiasing, true);

	//thepainter->drawRect(20, 120, 560, 560);

	connect(ui.plotbutton, SIGNAL(released()), this, SLOT(paintit()));
	//paintit();
}

ScatterPlot::~ScatterPlot()
{
	delete thepainter;
}

bool ScatterPlot::initialize(DataManage* the)
{

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
		ui.x_box->addItem(QString("Band") + QString::number(i + 1));
		ui.y_box->addItem(QString("Band") + QString::number(i + 1));
	}
	connect(ui.img_box, SIGNAL(currentIndexChanged(int)), this, SLOT(setbandnum(int)));

	return true;
}

bool ScatterPlot::paintit()
{
	if (tmanager->size() == 0)
	{
		QMessageBox::critical(NULL, "Error", "No image has been loaded!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		this->close();
	}

	thepainter->fillRect(20, 120, 560, 560, Qt::white);
	//thepainter->drawRect(20, 120, 560, 560);

	int imgnum = ui.img_box->currentIndex();
	int xband = ui.x_box->currentIndex();
	int yband = ui.y_box->currentIndex();

	Image* img = tmanager->getimg(imgnum);
	img->loadData(xband);
	img->loadData(yband);

	QPen penpoint;
	penpoint.setColor(Qt::black);
	penpoint.setWidth(1);
	thepainter->setPen(penpoint);

	QPoint startpo(20, 660);
	QPoint endx(20 + 512, 660);
	QPoint endy(20, 660 - 512);
	thepainter->drawLine(startpo, endx);
	thepainter->drawLine(startpo, endy);

	for (int i = 0; i < img->getRow(); ++i)
	{
		for (int j = 0; j < img->getCol(); ++j)
		{
			thepainter->drawPoint((img->at(xband, i, j) / 128) + 20, 660 - (img->at(yband, i, j) / 128));
		}
	}

	img->clear(xband);
	img->clear(yband);

	//»æÖÆ×ø±êÖá¿Ì¶È

	thepainter->drawText(10, 670, QString::number(0));
	thepainter->drawText(270, 690, QString("Band") + QString::number(xband+1));
	
	for (int i = 0; i < 8; ++i)
	{
		thepainter->drawText(5+64*(i+1), 675, QString::number(8192*(i+1)));
		QPoint s(20 + 64 * (i + 1), 660);
		QPoint e(20 + 64 * (i + 1), 650);
		thepainter->drawLine(s, e);
	}

	thepainter->rotate(-90);
	thepainter->drawText(-390, 35, QString("Band") + QString::number(yband+1));

	for (int i = 0; i < 8; ++i)
	{
		thepainter->drawText(-675+ 64 * (i + 1), 15, QString::number(8192 * (i + 1)));
		QPoint s(-660 + 64 * (i + 1), 20);
		QPoint e(-660 + 64 * (i + 1), 30);
		thepainter->drawLine(s, e);
	}

	thepainter->resetMatrix();

	update();
	return true;
}

void ScatterPlot::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.drawImage(0, 0, back_img);
}

bool ScatterPlot::setbandnum(int the)
{
	ui.x_box->clear();
	ui.y_box->clear();

	Image* img = tmanager->getimg(the);

	for (int i = 0; i < img->getBand(); ++i)
	{
		ui.x_box->addItem(QString("Band") + QString::number(i + 1));
		ui.y_box->addItem(QString("Band") + QString::number(i + 1));
	}

	return true;
}
