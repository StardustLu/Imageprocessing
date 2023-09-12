#pragma once

#include <QDialog>
#include <qmessagebox.h>
#include "ui_StaWindow.h"
#include "DataManage.h"


class StaWindow : public QDialog
{
	Q_OBJECT

public:
	StaWindow(QWidget *parent = Q_NULLPTR);
	~StaWindow();

	bool initialize(DataManage*);
	bool setValue();

private:

	DataManage* tmanager;

	Ui::StaWindow ui;
};
