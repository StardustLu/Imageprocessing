#include "ImageProcessing.h"

ImageProcessing::ImageProcessing(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	initialize();

}

ImageProcessing::~ImageProcessing()
{
	if (imgscene != nullptr) { delete imgscene; }

}

void ImageProcessing::toolTreeMenu(const QPoint&)
{
	cur_tree_item = ui.fileListTree->currentItem();  //获取当前被点击的节点
	if (cur_tree_item == nullptr)return;

	QString imgname = cur_tree_item->text(0);
	finddeleteimg(imgname);
	

	QAction deleteimg(QString::fromLocal8Bit("delete"), this);

	connect(&deleteimg, SIGNAL(triggered()), this, SLOT(deleteitem()));


	QPoint pos;
	QMenu menu(ui.fileListTree);
	menu.addAction(&deleteimg);
	menu.addAction(ui.actionOpen_RGB_image);
	menu.exec(QCursor::pos());

}

void ImageProcessing::deleteitem()
{
	if (deleteimg_index == -1)return;

	ui.fileListTree->takeTopLevelItem(deleteimg_index);
	
	if (imgisshowing)
	{
		imgscene->removeItem(cur_img_item);
		delete cur_img_item;
	}
	imgisshowing = false;

	QTreeWidgetItem* temp;
	int childnum = cur_tree_item->childCount();
	for (int i = 0; i < childnum; ++i)
	{
		temp = cur_tree_item->child(i);
		delete temp;
	}
	delete cur_tree_item;

	manager.cancel(deleteimg_index);

	deleteimg_index = -1;
	cur_tree_item = nullptr;
}

bool ImageProcessing::opentiff()
{
	//Image* theimg = manager.create();
	QFileDialog filedlg;
	QString fpath;

	fpath = filedlg.getOpenFileName(this, tr("Open Image"), ".", tr("Image Files(*.tif)"));
	//fpath = "F:/test/RSProcessing/data/theOsaka.tif";
	//fpath = "F:/test/RSProcessing/data/oneband.tif";

	if (fpath.isEmpty()) { return false; }
	else
	{
		Image* theimg = manager.create();
		theimg->getPath(fpath.toStdString());

		QString thename = QString::fromStdString(theimg->getName());

		QTreeWidgetItem* item = new QTreeWidgetItem(ui.fileListTree, QStringList(thename));//添加节点
		for (int i = 0; i < theimg->getBand(); i++)
		{
			QTreeWidgetItem* item1 = new QTreeWidgetItem(item, QStringList(QString("Band") + QString::number(i + 1)));
			item->addChild(item1);
		}

		ShowImage* showimg = manager.getshow(theimg);
		

		return true;
	}

}

bool ImageProcessing::loadRGBimg()
{

	//这里 info 的格式为	影像序号，R,G,B
	int info[4];
	for (int i = 0; i < 4; ++i)info[i] = 0;
	info[0] = -1;
	AddRGB rgbdlg;
	rgbdlg.initialize(&manager,info);
	rgbdlg.show();
	rgbdlg.exec();

	if (info[0] == -1) { return false; }
	manager.setCurrent(info[0]);

	ShowImage* showimg = manager.getshow(info[0]);
	showimg->setWindowSize(ui.mainview->geometry().width(), ui.mainview->geometry().height());

	showimg->loadData(info[1], info[2], info[3]);
	/*
	if (info[1] == info[2])
	{
		showimg->loadData(info[1]);
	}
	else
	{
		showimg->loadData(info[1], info[2], info[3]);
	}*/

	Enhance* imgenh = manager.getEnh(info[0]);
	imgenh->initialize();
	
	if (imgisshowing)
	{
		//imgenh->clearItem(imgscene);
		imgscene->removeItem(cur_img_item);
		delete cur_img_item;
	}

	//QGraphicsPixmapItem* simgitem= imgenh->linearStretch();
	cur_img_item = imgenh->linearStretch();
	imgscene->addItem(cur_img_item);
	imgisshowing = true;
	return true;
}

bool ImageProcessing::plotScatter()
{
	ScatterPlot scatter;
	scatter.initialize(&manager);
	scatter.show();
	scatter.exec();

	return true;
}

bool ImageProcessing::stawin()
{
	ui.progressBar->show();
	StaWindow staw;
	ui.progressBar->setValue(10);
	staw.initialize(&manager);
	ui.progressBar->setValue(20);
	staw.setValue();
	ui.progressBar->setValue(100);
	staw.show();
	staw.exec();
	ui.progressBar->hide();
	return true;
}

bool ImageProcessing::enhno()
{
	if (!imgisshowing)return false;

	int cur = manager.getCurrent();
	ShowImage* showimg = manager.getshow(cur);
	showimg->loadCurrent();

	Enhance* imgenh = manager.getEnh(cur);
	//imgenh->clearItem(imgscene);
	if (imgisshowing)
	{
		imgscene->removeItem(cur_img_item);
		delete cur_img_item;
	}
	//QGraphicsPixmapItem* simgitem = imgenh->noStretch();
	cur_img_item = imgenh->noStretch();
	imgscene->addItem(cur_img_item);

	return true;
}

bool ImageProcessing::enhLinear1()
{
	if (!imgisshowing)return false;

	int cur=manager.getCurrent();
	ShowImage* showimg = manager.getshow(cur);
	showimg->loadCurrent();

	Enhance* imgenh = manager.getEnh(cur);
	//imgenh->clearItem(imgscene);
	//QGraphicsPixmapItem* simgitem = imgenh->linearStretch(1.0);
	if (imgisshowing)
	{
		imgscene->removeItem(cur_img_item);
		delete cur_img_item;
	}
	//QGraphicsPixmapItem* simgitem = imgenh->noStretch();
	cur_img_item = imgenh->linearStretch(1);
	imgscene->addItem(cur_img_item);

	return true;
}

bool ImageProcessing::enhLinear2()
{
	if (!imgisshowing)return false;

	int cur = manager.getCurrent();
	ShowImage* showimg = manager.getshow(cur);
	showimg->loadCurrent();

	Enhance* imgenh = manager.getEnh(cur);
	//imgenh->clearItem(imgscene);
	//QGraphicsPixmapItem* simgitem = imgenh->linearStretch(2.0);
	//imgscene->addItem(simgitem);
	if (imgisshowing)
	{
		imgscene->removeItem(cur_img_item);
		delete cur_img_item;
	}
	cur_img_item = imgenh->linearStretch(2);
	imgscene->addItem(cur_img_item);

	return true;
}

bool ImageProcessing::enhLinear5()
{
	if (!imgisshowing)return false;

	int cur = manager.getCurrent();
	ShowImage* showimg = manager.getshow(cur);
	showimg->loadCurrent();

	Enhance* imgenh = manager.getEnh(cur);
	//imgenh->clearItem(imgscene);
	//QGraphicsPixmapItem* simgitem = imgenh->linearStretch(5.0);
	//imgscene->addItem(simgitem);
	if (imgisshowing)
	{
		imgscene->removeItem(cur_img_item);
		delete cur_img_item;
	}
	cur_img_item = imgenh->linearStretch(5);
	imgscene->addItem(cur_img_item);

	return true;
}

bool ImageProcessing::enhLog()
{
	if (!imgisshowing)return false;

	int cur = manager.getCurrent();
	ShowImage* showimg = manager.getshow(cur);
	showimg->loadCurrent();

	Enhance* imgenh = manager.getEnh(cur);
	//imgenh->clearItem(imgscene);
	//QGraphicsPixmapItem* simgitem = imgenh->logStretch();
	//imgscene->addItem(simgitem);

	if (imgisshowing)
	{
		imgscene->removeItem(cur_img_item);
		delete cur_img_item;
	}
	cur_img_item = imgenh->logStretch();
	imgscene->addItem(cur_img_item);

	return true;
}

bool ImageProcessing::enhhis()
{
	if (!imgisshowing)return false;

	int cur = manager.getCurrent();
	ShowImage* showimg = manager.getshow(cur);
	showimg->loadCurrent();

	Enhance* imgenh = manager.getEnh(cur);
	//imgenh->clearItem(imgscene);
	//QGraphicsPixmapItem* simgitem = imgenh->hisEqualization();
	//imgscene->addItem(simgitem);
	if (imgisshowing)
	{
		imgscene->removeItem(cur_img_item);
		delete cur_img_item;
	}
	cur_img_item = imgenh->hisEqualization();
	imgscene->addItem(cur_img_item);
	return true;
}

bool ImageProcessing::PCAtransform()
{
	bool doit = false;
	int index = -1;
	string invpath;
	string forpath;
	QString winname = "PCA Transform";

	PCAdlg dlg;
	dlg.initialize(&manager, &doit, &index,&forpath,&invpath,winname);
	dlg.show();
	dlg.exec();

	if (index == -1 || doit == false)return false;
	if (forpath.empty())return false;
	if (invpath.empty())return false;

	ui.progressBar->show();
	Image* img = manager.getimg(index);
	PCATrans trans(&manager, img);
	trans.initalize();
	ui.progressBar->setValue(10);
	trans.calTransMat();
	ui.progressBar->setValue(30);

	Image* forimg=trans.PCAforward(forpath);
	ui.progressBar->setValue(75);

	QString thename = QString::fromStdString(forimg->getName());
	QTreeWidgetItem* item = new QTreeWidgetItem(ui.fileListTree, QStringList(thename));//添加节点
	for (int i = 0; i < forimg->getBand(); i++)
	{
		QTreeWidgetItem* item1 = new QTreeWidgetItem(item, QStringList(QString("Band") + QString::number(i + 1)));
		item->addChild(item1);
	}

	Image* invimg=trans.PCAinverse(invpath);

	QString thename2 = QString::fromStdString(invimg->getName());
	QTreeWidgetItem* item2 = new QTreeWidgetItem(ui.fileListTree, QStringList(thename2));//添加节点
	for (int i = 0; i < invimg->getBand(); i++)
	{
		QTreeWidgetItem* item12 = new QTreeWidgetItem(item2, QStringList(QString("Band") + QString::number(i + 1)));
		item2->addChild(item12);
	}
	ui.progressBar->setValue(100);
	MatrixXd* transmat = trans.getTansMat();
	PCAmat matdlg;
	matdlg.initialize(transmat);
	matdlg.show();
	matdlg.exec();
	ui.progressBar->hide();
	return true;
}

bool ImageProcessing::ISODataClf()
{
	bool doit = false;
	int index = -1;
	string outpath;
	int classnum;
	int max_iter;
	int min_num;
	int max_stdv;
	int min_dis;

	ISODATAdlg dlg;
	dlg.initialize(&manager,&index,&doit, &outpath, &classnum, &max_iter, &min_num, &max_stdv, &min_dis);
	dlg.show();
	dlg.exec();

	if (index == -1 || doit == false)return false;
	if (outpath.empty())return false;

	ui.progressBar->show();
	Image* img = manager.getimg(index);
	ISODATA clf(&manager, img);
	clf.initialize(outpath);
	ui.progressBar->setValue(10);
	Image* result=clf.classify(2 * classnum, classnum, min_num, max_stdv,min_dis, max_iter);
	ui.progressBar->setValue(90);

	QString thename = QString::fromStdString(result->getName());
	QTreeWidgetItem* item = new QTreeWidgetItem(ui.fileListTree, QStringList(thename));//添加节点
	for (int i = 0; i < result->getBand(); i++)
	{
		QTreeWidgetItem* item1 = new QTreeWidgetItem(item, QStringList(QString("Band") + QString::number(i + 1)));
		item->addChild(item1);
	}
	ui.progressBar->setValue(100);
	QMessageBox::information(NULL, "ISODATA", "ISODATA completed!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	ui.progressBar->hide();
	return true;
}

bool ImageProcessing::OtsuSegmentation()
{
	bool doit = false;
	int index = -1;
	int band = -1;
	string outpath;
	QString windowname = "Otsu Segmentaion";

	Otsudlg dlg;
	dlg.initialize(&manager, &doit, &index,&band, &outpath,windowname);
	dlg.show();
	dlg.exec();

	if (index == -1 || doit == false)return false;
	if (outpath.empty())return false;
	if (band == 0)band = -1;

	ui.progressBar->show();
	Image* img = manager.getimg(index);
	OtsuSeg seg(&manager, img);
	ui.progressBar->setValue(10);
	seg.initialize();
	ui.progressBar->setValue(20);
	seg.calThreshold();
	ui.progressBar->setValue(30);
	Image* result=seg.splitit(outpath,band);
	ui.progressBar->setValue(95);

	QString thename = QString::fromStdString(result->getName());
	QTreeWidgetItem* item = new QTreeWidgetItem(ui.fileListTree, QStringList(thename));//添加节点
	for (int i = 0; i < result->getBand(); i++)
	{
		QTreeWidgetItem* item1 = new QTreeWidgetItem(item, QStringList(QString("Band") + QString::number(i + 1)));
		item->addChild(item1);
	}
	ui.progressBar->setValue(100);
	QMessageBox::information(NULL, "Otsu Segmentaion", "Segmentaion completed!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	ui.progressBar->hide();
	return true;
}

bool ImageProcessing::Fourier()
{
	bool doit = false;
	int index = -1;
	string invpath;
	string forpath;
	QString winname = "Fourier Transform";

	PCAdlg dlg;
	dlg.initialize(&manager, &doit, &index, &forpath, &invpath, winname);
	dlg.show();
	dlg.exec();

	if (index == -1 || doit == false)return false;
	if (forpath.empty())return false;
	if (invpath.empty())return false;

	ui.progressBar->show();

	Image* img = manager.getimg(index);

	FourierTrans trans(&manager, img);
	ui.progressBar->setValue(10);
	Image* forimg = trans.fftForward(forpath);
	ui.progressBar->setValue(60);
	QString thename = QString::fromStdString(forimg->getName());
	QTreeWidgetItem* item = new QTreeWidgetItem(ui.fileListTree, QStringList(thename));//添加节点
	for (int i = 0; i < forimg->getBand(); i++)
	{
		QTreeWidgetItem* item1 = new QTreeWidgetItem(item, QStringList(QString("Band") + QString::number(i + 1)));
		item->addChild(item1);
	}

	Image* invimg = trans.fftInverse(invpath);
	ui.progressBar->setValue(95);
	QString thename2 = QString::fromStdString(invimg->getName());
	QTreeWidgetItem* item2 = new QTreeWidgetItem(ui.fileListTree, QStringList(thename2));//添加节点
	for (int i = 0; i < invimg->getBand(); i++)
	{
		QTreeWidgetItem* item12 = new QTreeWidgetItem(item2, QStringList(QString("Band") + QString::number(i + 1)));
		item2->addChild(item12);
	}
	ui.progressBar->setValue(100);
	QMessageBox::information(NULL, "Fourier Transform", "Fourier Transform completed!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	ui.progressBar->hide();
	return true;
}

bool ImageProcessing::sampleJM()
{
	vector<vector<string>> path;
	path.resize(3);
	for (int i = 0; i < 3; ++i)
	{
		path[i].resize(3);
	}
	path[0][0] = "F:/test/RSProcessing/data/sample/forest1.tif";
	path[0][1] = "F:/test/RSProcessing/data/sample/forest2.tif";
	path[0][2] = "F:/test/RSProcessing/data/sample/forest3.tif";
	path[1][0] = "F:/test/RSProcessing/data/sample/ground1.tif";
	path[1][1] = "F:/test/RSProcessing/data/sample/ground2.tif";
	path[1][2] = "F:/test/RSProcessing/data/sample/ground3.tif";
	path[2][0] = "F:/test/RSProcessing/data/sample/ocean1.tif";
	path[2][1] = "F:/test/RSProcessing/data/sample/ocean2.tif";
	path[2][2] = "F:/test/RSProcessing/data/sample/ocean3.tif";
	vector<vector<Image>> thesample;
	thesample.resize(3);
	for (int i = 0; i < 3; ++i)
	{
		thesample[i].resize(3);
		for (int j = 0; j < thesample[i].size(); ++j)
		{
			thesample[i][j].getPath(path[i][j]);
		}
	}
	SampleProcess samplemanage(&thesample);

	MatrixXd* JMmat = samplemanage.roiJudge();
	map<int, string> label;
	label[0] = "Forest";
	label[1] = "Ground";
	label[2] = "Ocean";

	PCAmat dlg;
	dlg.showconfusion(JMmat, label);
	dlg.show();
	dlg.exec();

	return true;
}

bool ImageProcessing::SVMclf()
{
	bool doit = false;
	int index = -1;
	string outpath;
	int kernel;
	double c;
	double gamma;
	int d;

	SVMdlg dlg;
	dlg.initialize(&manager, &index, &doit, &outpath, &kernel, &c, &gamma, &d);
	dlg.show();
	dlg.exec();
	if (index == -1 || doit == false)return false;
	if (outpath.empty())return false;

	ui.progressBar->show();
	Image* img = manager.getimg(index);
	//这一段是临时代码，因为没有实现通过鼠标选取样本
	vector<vector<string>> path;
	path.resize(3);
	for (int i = 0; i < 3; ++i)
	{
		path[i].resize(3);
	}
	path[0][0] = "F:/test/RSProcessing/data/sample/forest1.tif";
	path[0][1] = "F:/test/RSProcessing/data/sample/forest2.tif";
	path[0][2] = "F:/test/RSProcessing/data/sample/forest3.tif";
	path[1][0] = "F:/test/RSProcessing/data/sample/ground1.tif";
	path[1][1] = "F:/test/RSProcessing/data/sample/ground2.tif";
	path[1][2] = "F:/test/RSProcessing/data/sample/ground3.tif";
	path[2][0] = "F:/test/RSProcessing/data/sample/ocean1.tif";
	path[2][1] = "F:/test/RSProcessing/data/sample/ocean2.tif";
	path[2][2] = "F:/test/RSProcessing/data/sample/ocean3.tif";
	ui.progressBar->setValue(10);
	vector<vector<Image>> thesample;
	thesample.resize(3);
	for (int i = 0; i < 3; ++i)
	{
		thesample[i].resize(1);
		for (int j = 0; j < thesample[i].size(); ++j)
		{
			thesample[i][j].getPath(path[i][j]);
		}
	}
	SampleProcess samplemanage(&thesample);
	vector<vector<VectorXd>>* sample = samplemanage.getSample();
	vector<vector<VectorXd>>* test = samplemanage.getTest();
	ui.progressBar->setValue(20);
	SVMClassifier clf(&manager, img);
	clf.getSample(sample);
	KernelFunction type;
	switch (kernel)
	{
	case 0:
		type = Linear;
		break;
	case 1:
		type = Polynomial;
		break;
	case 2:
		type = RBF;
		break;
	}

	clf.train(type, c, gamma, d);
	ui.progressBar->setValue(60);
	Image* result = clf.classify(outpath);
	ui.progressBar->setValue(95);
	QString thename = QString::fromStdString(result->getName());
	QTreeWidgetItem* item = new QTreeWidgetItem(ui.fileListTree, QStringList(thename));//添加节点
	for (int i = 0; i < result->getBand(); i++)
	{
		QTreeWidgetItem* item1 = new QTreeWidgetItem(item, QStringList(QString("Band") + QString::number(i + 1)));
		item->addChild(item1);
	}
	map<int, string> label;
	label[0] = "Forest";
	label[1] = "Ground";
	label[2] = "Ocean";
	MatrixXd* confusion = clf.getConfusionMatrix(test);
	ui.progressBar->setValue(100);
	PCAmat matdlg;
	matdlg.showconfusion(confusion, label);
	matdlg.show();
	matdlg.exec();
	ui.progressBar->hide();
	return true;
}

bool ImageProcessing::BPNetworkclf()
{
	bool doit = false;
	int index = -1;
	string outpath;
	int layers;
	int nureons;
	double learning_rate;
	int max_iter;

	Networkdlg dlg;
	dlg.initialize(&manager, &doit, &index, &outpath, &layers, &nureons, &learning_rate, &max_iter);
	dlg.show();
	dlg.exec();
	if (index == -1 || doit == false)return false;
	if (outpath.empty())return false;
	ui.progressBar->show();
	Image* img = manager.getimg(index);
	//临时样本
	vector<vector<string>> path;
	path.resize(3);
	for (int i = 0; i < 3; ++i)
	{
		path[i].resize(1);
	}
	path[2][0] = "F:/test/RSProcessing/data/sample/ocean6.tif";
	path[1][0] = "F:/test/RSProcessing/data/sample/forest6.tif";
	path[0][0] = "F:/test/RSProcessing/data/sample/ground6.tif";
	vector<vector<Image>> thesample;
	thesample.resize(3);
	for (int i = 0; i < 3; ++i)
	{
		thesample[i].resize(1);
		for (int j = 0; j < thesample[i].size(); ++j)
		{
			thesample[i][j].getPath(path[i][j]);
		}
	}
	SampleProcess samplemanage(&thesample);
	vector<vector<VectorXd>>* sample = samplemanage.getSample();
	vector<vector<VectorXd>>* test = samplemanage.getTest();
	ui.progressBar->setValue(5);
	vector<vector<VectorXd>> temp;
	vector<vector<VectorXd>> temptest;
	temp.resize(3);
	temptest.resize(3);
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 100; ++j)
		{
			VectorXd ttemp(3);
			ttemp(0) = (*sample)[i][j](2);
			ttemp(1) = (*sample)[i][j](3);
			ttemp(2) = (*sample)[i][j](4);
			temp[i].push_back(ttemp);
			ttemp(0) = (*test)[i][j](2);
			ttemp(1) = (*test)[i][j](3);
			ttemp(2) = (*test)[i][j](4);
			temptest[i].push_back(ttemp);
		}
	}
	//
	ui.progressBar->setValue(10);
	BPNet clf(&manager, img);
	clf.initialize(30, 2, 0.5, 200, sgd);
	ui.progressBar->setValue(20);
	clf.train(&temp);
	ui.progressBar->setValue(60);

	Image* result=clf.classify(outpath);
	ui.progressBar->setValue(90);
	QString thename = QString::fromStdString(result->getName());
	QTreeWidgetItem* item = new QTreeWidgetItem(ui.fileListTree, QStringList(thename));//添加节点
	for (int i = 0; i < result->getBand(); i++)
	{
		QTreeWidgetItem* item1 = new QTreeWidgetItem(item, QStringList(QString("Band") + QString::number(i + 1)));
		item->addChild(item1);
	}
	ui.progressBar->setValue(95);
	map<int, string> label;
	label[0] = "Ground";
	label[1] = "Forest";
	label[2] = "Ocean";
	MatrixXd* confusion = clf.getConfusionMatrix(&temptest);
	ui.progressBar->setValue(100);
	PCAmat matdlg;
	matdlg.showconfusion(confusion, label);
	matdlg.show();
	matdlg.exec();
	ui.progressBar->hide();

	return true;
}

bool ImageProcessing::majority()
{
	bool doit = false;
	int index = -1;
	string outpath;
	int kernel_row, kernel_col;

	Filterdlg dlg;
	dlg.initialize(&manager, &doit, &index, &outpath,&kernel_row,&kernel_col);
	dlg.show();
	dlg.exec();
	if (index == -1 || doit == false)return false;
	if (outpath.empty())return false;
	ui.progressBar->show();
	Image* img = manager.getimg(index);

	Filter flt(&manager);
	ui.progressBar->setValue(10);
	Image* result=flt.majority(img, outpath, kernel_row);
	ui.progressBar->setValue(90);
	QString thename = QString::fromStdString(result->getName());
	QTreeWidgetItem* item = new QTreeWidgetItem(ui.fileListTree, QStringList(thename));//添加节点
	for (int i = 0; i < result->getBand(); i++)
	{
		QTreeWidgetItem* item1 = new QTreeWidgetItem(item, QStringList(QString("Band") + QString::number(i + 1)));
		item->addChild(item1);
	}
	ui.progressBar->setValue(100);
	QMessageBox::information(NULL, "Majority", "Majority Analysis completed!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

	ui.progressBar->hide();

	return true;
}

void ImageProcessing::toolTreeProcess(QTreeWidgetItem* item, int index)
{

	QString temp = item->text(0);

	if (temp == QString("Scatter"))
	{
		plotScatter();
		return;
	}
	else if (temp == QString("PCA Analysis"))
	{
		PCAtransform();
		return;
	}
	else if (temp == QString("JM Distance"))
	{
		sampleJM();
		return;
	}
	else if (temp == QString("SVM"))
	{
		SVMclf();
		return;
	}
	else if (temp == QString("Network"))
	{
		BPNetworkclf();
		return;
	}
	else if (temp == QString("ISODATA"))
	{
		ISODataClf();
		return;
	}
	else if (temp == QString("Majority"))
	{
		majority();
		return;
	}
	else if (temp == QString("Statistic"))
	{
		stawin();
		return;
	}
	else if (temp == QString("Segmentation"))
	{
		OtsuSegmentation();
		return;
	}
	else if (temp == QString("FFT"))
	{
		Fourier();
		return;
	}
	else
	{
		return;
	}

	return;
}

bool ImageProcessing::initialize()
{
	imgscene = new QGraphicsScene();
	ui.mainview->setScene(imgscene);
	imgisshowing = false;
	ui.progressBar->hide();

	connect(ui.actionOpen_GeoTiff, &QAction::triggered, this, &ImageProcessing::opentiff);
	connect(ui.actionOpen_RGB_image, &QAction::triggered, this, &ImageProcessing::loadRGBimg);
	connect(ui.actionScatter, &QAction::triggered, this, &ImageProcessing::plotScatter);
	connect(ui.actionBasic_Statistic, &QAction::triggered, this, &ImageProcessing::stawin);
	connect(ui.action1_stretch, &QAction::triggered, this, &ImageProcessing::enhLinear1);
	connect(ui.action2_stretch, &QAction::triggered, this, &ImageProcessing::enhLinear2);
	connect(ui.action5_stretch, &QAction::triggered, this, &ImageProcessing::enhLinear5);
	connect(ui.actionhistogram_Equalization, &QAction::triggered, this, &ImageProcessing::enhhis);
	connect(ui.actionLogarithmic, &QAction::triggered, this, &ImageProcessing::enhLog);
	connect(ui.actionPCA, &QAction::triggered, this, &ImageProcessing::PCAtransform);
	connect(ui.actionSegmentation, &QAction::triggered, this, &ImageProcessing::OtsuSegmentation);
	connect(ui.actionFFT, &QAction::triggered, this, &ImageProcessing::Fourier);
	connect(ui.actionISODATA, &QAction::triggered, this, &ImageProcessing::ISODataClf);
	connect(ui.actionSVM, &QAction::triggered, this, &ImageProcessing::SVMclf);
	connect(ui.actionBPnet, &QAction::triggered, this, &ImageProcessing::BPNetworkclf);
	connect(ui.actionJM, &QAction::triggered, this, &ImageProcessing::sampleJM);
	connect(ui.actionMajority, &QAction::triggered, this, &ImageProcessing::majority);

	connect(ui.tooltree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(toolTreeProcess(QTreeWidgetItem*, int)));
	//connect(ui.fileListTree, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(toolTreeMenu(const QPoint&)));
	ui.fileListTree->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.fileListTree, &QTreeView::customContextMenuRequested, this, &ImageProcessing::toolTreeMenu);

	return true;
}

bool ImageProcessing::finddeleteimg(QString& thename)
{
	Image* img;
	for (int i = 0; i < manager.size(); ++i)
	{
		img = manager.getimg(i);
		QString temp = QString::fromStdString(img->getName());

		if (temp == thename)
		{
			deleteimg_index = i;
			return true;
		}

	}
	return false;
}
