#pragma once

#include <QDialog>
#include "ui_SVMdlg.h"
#include "DataManage.h"
#include <qfiledialog.h>

class SVMdlg : public QDialog
{
	Q_OBJECT

public:
	SVMdlg(QWidget *parent = Q_NULLPTR);
	~SVMdlg();

	bool initialize(DataManage* tm, int* index, bool* thedo, string* thepath, int* thekernel, double* thec, double* thegamma, int* thed);

private slots :

	bool oksvmButton();
	bool cansvmButton();
	bool filesvmButton();

private:
	Ui::SVMdlg ui;

	DataManage* manager;
	int* theindex;
	bool* doti;
	string* opath;
	int* kernel;
	double* c;
	double* gamma;
	int* d;

};
