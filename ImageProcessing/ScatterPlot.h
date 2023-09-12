#pragma once

#include <QDialog>
#include "ui_ScatterPlot.h"
#include <qimage.h>
#include <qpainter.h>
#include <qmessagebox.h>
#include "DataManage.h"

class ScatterPlot : public QDialog
{
	Q_OBJECT

public:
	ScatterPlot(QWidget *parent = Q_NULLPTR);
	~ScatterPlot();

	bool initialize(DataManage*);
	

protected:
	void paintEvent(QPaintEvent*);

private slots:
	bool paintit();
	bool setbandnum(int);
private:

	int xyband[2];

	DataManage* tmanager;
	QImage back_img;
	QPainter* thepainter;

	Ui::ScatterPlot ui;

};
