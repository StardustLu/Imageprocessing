#pragma once

#include <QDialog>
#include "ui_ISODATAdlg.h"
#include "DataManage.h"
#include <qfiledialog.h>

class ISODATAdlg : public QDialog
{
	Q_OBJECT

public:
	ISODATAdlg(QWidget *parent = Q_NULLPTR);
	~ISODATAdlg();

	bool initialize(DataManage*,int*,bool* tdoit,string* opath,int* class_num,int* max_it,int* minnum,int* max_std,int* mindis);

private slots:

	bool okisoButton();
	bool caisoButton();
	bool fileisoButton();

private:

	DataManage* manager;
	int* theindex;
	bool* doit;
	string* outpath;
	int* classnum;
	int* max_iter;
	int* min_num;
	int* max_stdv;
	int* min_dis;

	Ui::ISODATAdlg ui;
};
