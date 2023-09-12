#pragma once

#include <QDialog>
#include "ui_Otsudlg.h"
#include "DataManage.h"
#include <qfiledialog.h>

class Otsudlg : public QDialog
{
	Q_OBJECT

public:
	Otsudlg(QWidget *parent = Q_NULLPTR);
	~Otsudlg();

	bool initialize(DataManage*, bool*, int*,int*, string*,QString&);

private slots:

	bool okOtsuButton();
	bool caOtsuButton();
	bool fileOtsuButton();
	bool setband(int);

private:
	Ui::Otsudlg ui;

	DataManage* manager;
	bool* doit;
	int* num;
	int* tband;
	string* path;
};
