#include "PCAmat.h"

PCAmat::PCAmat(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

PCAmat::~PCAmat()
{
}

bool PCAmat::initialize(MatrixXd* trans)
{
	ui.tableWidget->setRowCount(trans->rows());
	ui.tableWidget->setColumnCount(trans->cols());

	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

	for (int i = 0; i < trans->rows(); ++i)
	{
		for (int j = 0; j < trans->cols(); ++j)
		{
			ui.tableWidget->setItem(i, j, new QTableWidgetItem(QString::number((*trans)(i, j))));
		}
	}

	return true;
}

bool PCAmat::showconfusion(MatrixXd* conf,map<int,string>& label)
{
	ui.tableWidget->setRowCount(conf->rows()+1);
	ui.tableWidget->setColumnCount(conf->cols()+1);
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

	for (int i = 1; i < conf->rows() + 1; ++i)
	{
		ui.tableWidget->setItem(i , 0, new QTableWidgetItem(QString::fromStdString(label[i-1])));
		ui.tableWidget->setItem(0, i , new QTableWidgetItem(QString::fromStdString(label[i-1])));
		for (int j = 1; j < conf->cols() + 1; ++j)
		{
			ui.tableWidget->setItem(i, j, new QTableWidgetItem(QString::number((*conf)(i-1, j-1))));
		}
	}

	ui.tableWidget->setItem(0, 0, new QTableWidgetItem(QString("Truth/Predict")));
	
	return true;
}
