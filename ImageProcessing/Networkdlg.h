#pragma once

#include <QDialog>
#include "ui_Networkdlg.h"
#include "DataManage.h"
#include <qfiledialog.h>

class Networkdlg : public QDialog
{
	Q_OBJECT

public:
	Networkdlg(QWidget *parent = Q_NULLPTR);
	~Networkdlg();

	bool initialize(DataManage* tm,bool* doit,int* index,string* outpath,int* layers,int* nurons,double* learning_rate,int* max_iter);

private slots:

	bool okNButton();
	bool canNButton();
	bool fileNButton();
private:
	Ui::Networkdlg ui;

	DataManage* manager;
	bool* tdoit;
	string* opath;
	int* theindex;
	int* thelayers;
	int* thenuuros;
	double* thelearnrate;
	int* themax;

};
