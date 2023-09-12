#include "StaWindow.h"

StaWindow::StaWindow(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

StaWindow::~StaWindow()
{
}

bool StaWindow::initialize(DataManage* the)
{
	tmanager = the;
	setWindowTitle("Basic Statistics");

	if (tmanager->size() == 0)
	{
		QMessageBox::critical(NULL, "Error", "No image has been loaded!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		this->close();
	}


	return true;
}

bool StaWindow::setValue()
{
	int index = tmanager->getCurrent();
	Statistic* sta = tmanager->getSta(index);
	sta->initialize();
	Image* img = tmanager->getimg(index);

	ui.tableWidget->setRowCount(10 + 2 * (img->getBand()));
	ui.tableWidget->setColumnCount(img->getBand() + 1);

	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

	for (int i = 0; i < img->getBand(); ++i)
	{
		ui.tableWidget->setItem(0, i + 1, new QTableWidgetItem(QString("Band") + QString::number(i + 1)));
		ui.tableWidget->setItem(7, i + 1, new QTableWidgetItem(QString("Band") + QString::number(i + 1)));
		ui.tableWidget->setItem(9 + img->getBand(), i + 1, new QTableWidgetItem(QString("Band") + QString::number(i + 1)));
		ui.tableWidget->setItem(8 + i, 0, new QTableWidgetItem(QString("Band") + QString::number(i + 1)));
		ui.tableWidget->setItem(10 + img->getBand() + i, 0, new QTableWidgetItem(QString("Band") + QString::number(i + 1)));

	}

	ui.tableWidget->setItem(1, 0, new QTableWidgetItem(QString("Min")));
	ui.tableWidget->setItem(2, 0, new QTableWidgetItem(QString("Max")));
	ui.tableWidget->setItem(3, 0, new QTableWidgetItem(QString("Mean")));
	if (!img->isFloat())
	{
		ui.tableWidget->setItem(4, 0, new QTableWidgetItem(QString("Median")));
		ui.tableWidget->setItem(5, 0, new QTableWidgetItem(QString("Mode")));
	}
	ui.tableWidget->setItem(7, 0, new QTableWidgetItem(QString("Covariance Matrix")));
	ui.tableWidget->setItem(9+img->getBand(), 0, new QTableWidgetItem(QString("Correlation Matrix")));

	vector<double>* minv = sta->minVec();
	vector<double>* maxv = sta->maxVec();
	vector<double>* meanv = sta->meanVec();
	vector<double>* midv = sta->midVec();
	vector<double>* modev = sta->modeVec();

	for (int i = 0; i < img->getBand(); ++i)
	{
		ui.tableWidget->setItem(1, i + 1, new QTableWidgetItem(QString::number((*minv)[i])));
		ui.tableWidget->setItem(2, i + 1, new QTableWidgetItem(QString::number((*maxv)[i])));
		ui.tableWidget->setItem(3, i + 1, new QTableWidgetItem(QString::number((*meanv)[i])));
		if (!img->isFloat())
		{
			ui.tableWidget->setItem(4, i + 1, new QTableWidgetItem(QString::number((*midv)[i])));
			ui.tableWidget->setItem(5, i + 1, new QTableWidgetItem(QString::number((*modev)[i])));
		}

	}

	MatrixXd* cov = sta->coVarMat();
	MatrixXd* cor = sta->correlationMat();

	for (int i = 0; i < img->getBand(); ++i)
	{
		for (int j = 0; j < img->getBand(); ++j)
		{
			ui.tableWidget->setItem(8 + i, 1 + j, new QTableWidgetItem(QString::number((*cov)(i, j))));
			ui.tableWidget->setItem(10 + i + img->getBand(), 1 + j, new QTableWidgetItem(QString::number((*cor)(i, j))));
		}
	}


	return true;
}
