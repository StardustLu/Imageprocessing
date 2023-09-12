#pragma once

#include <QGraphicsView>

class Imageview : public QGraphicsView
{
	Q_OBJECT

public:
	Imageview(QWidget *parent);
	~Imageview();
};
