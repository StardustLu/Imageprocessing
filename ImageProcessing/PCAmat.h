#pragma once

#include <QDialog>
#include "ui_PCAmat.h"
#include "DataManage.h"
#include <map>

class PCAmat : public QDialog
{
	Q_OBJECT

public:
	PCAmat(QWidget *parent = Q_NULLPTR);
	~PCAmat();

	bool initialize(MatrixXd*);
	bool showconfusion(MatrixXd*,map<int,string>&);
	

private:
	Ui::PCAmat ui;
};
