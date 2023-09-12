#pragma once


#include <QtWidgets/QMainWindow>
#include <qfiledialog.h>
#include <qstring.h>
#include <qtreewidget.h>
#include <qvector.h>
#include <qgraphicsscene.h>
#include <qgraphicsview.h>
#include <qgraphicsitem.h>
#include <qpoint.h>
#include <qmenu.h>
#include "ui_ImageProcessing.h"

#include "DataManage.h"
#include "AddRGB.h"
#include "ScatterPlot.h"
#include "StaWindow.h"
#include "PCATrans.h"
#include "PCAdlg.h"
#include "PCAmat.h"
#include "Otsudlg.h"
#include "OtsuSeg.h"
#include "FourierTrans.h"
#include "ISODATAdlg.h"
#include "ISODATA.h"
#include "SVMdlg.h"
#include "SVMClassifier.h"
#include "SampleProcess.h"
#include "Networkdlg.h"
#include "BPNet.h"
#include "Filterdlg.h"
#include "Filter.h"

class ImageProcessing : public QMainWindow
{
	Q_OBJECT

public:
	ImageProcessing(QWidget *parent = Q_NULLPTR);
	~ImageProcessing();

private slots:

	void toolTreeProcess(QTreeWidgetItem*, int);
	void toolTreeMenu(const QPoint&);
	void deleteitem();

private:

	//²Ûº¯Êý
	bool opentiff();
	bool loadRGBimg();
	bool plotScatter();
	bool stawin();
	bool enhno();
	bool enhLinear1();
	bool enhLinear2();
	bool enhLinear5();
	bool enhLog();
	bool enhhis();
	bool PCAtransform();
	bool ISODataClf();
	bool OtsuSegmentation();
	bool Fourier();
	bool sampleJM();
	bool SVMclf();
	bool BPNetworkclf();
	bool majority();

	//

	

	bool initialize();
	bool finddeleteimg(QString&);

	bool imgisshowing;
	int deleteimg_index;
	QTreeWidgetItem* cur_tree_item;
	QGraphicsPixmapItem* cur_img_item;

	DataManage manager;

	Ui::ImageProcessingClass ui;
	QGraphicsScene* imgscene;
};
