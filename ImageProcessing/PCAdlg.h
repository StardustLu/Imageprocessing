#pragma once

#include <QDialog>
#include "ui_PCAdlg.h"
#include "DataManage.h"
#include <qfiledialog.h>

class PCAdlg : public QDialog
{
	Q_OBJECT

public:
	PCAdlg(QWidget *parent = Q_NULLPTR);
	~PCAdlg();

	bool initialize(DataManage*,bool*,int*,string*,string*,QString&);

private slots:

	bool okpcaButton();
	bool capcaButton();
	bool forbut();
	bool invbut();

private:
	Ui::PCAdlg ui;

	DataManage* manager;
	bool* doit;
	int* num;
	string* forpath;
	string* invpath;
};
