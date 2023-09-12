#pragma once

#include <QDialog>
#include "ui_Filterdlg.h"
#include "DataManage.h"
#include <qfiledialog.h>

class Filterdlg : public QDialog
{
	Q_OBJECT

public:
	Filterdlg(QWidget *parent = Q_NULLPTR);
	~Filterdlg();

	bool initialize(DataManage* tm,bool* doit,int* index,string* outpath,int* kernel_row,int* kernel_col);

private slots:

	bool okFButton();
	bool caFButton();
	bool fileFButton();

private:
	Ui::Filterdlg ui;

	DataManage* manager;
	string* opath;
	bool* tdoti;
	int* tindex;
	int* krow;
	int* kcol;

};
