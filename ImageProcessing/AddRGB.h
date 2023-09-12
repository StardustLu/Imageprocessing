#pragma once

#include <QDialog>
#include "ui_AddRGB.h"
#include "DataManage.h"
#include <qmessagebox.h>

class AddRGB : public QDialog
{
	Q_OBJECT

public:
	AddRGB(QWidget *parent = Q_NULLPTR);
	~AddRGB();

	bool initialize(DataManage* the,int* info);

private slots:

	bool okButton();
	bool caButton();
	bool setbandnum(int);

private:



	DataManage* tmanager;
	int* theinfo;
	int currentimg;

	Ui::AddRGB ui;
};
