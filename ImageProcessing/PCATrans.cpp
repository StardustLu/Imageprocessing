#include "PCATrans.h"

bool PCATrans::initalize()
{
	sta = tmanager->getSta(img);
	if (sta == nullptr) { return false; }
	sta->initialize();

	return true;
}

Image* PCATrans::PCAforward(string& outpath)
{
	Image* pcaimg = tmanager->create();
	pcaimg->create(img->getBand(), img->getRow(), img->getCol(), img, outpath, true);

	for (int i = 0; i < img->getBlockRow(); ++i)
	{
		for (int j = 0; j < img->getBlockCol(); ++j)
		{
			img->loadDatabyBlock(i, j);
			pcaimg->prepareDatabyBlock(i, j);

			for (int a = 0; a < img->getEachBlockRow(); ++a)
			{
				for (int b = 0; b < img->getEachBlockCol(); ++b)
				{
					VectorXd temp(img->getBand());

					for (int k = 0; k < img->getBand(); ++k)
					{
						temp(k) = img->atBlock(k, a, b);
					}

					VectorXd result = forward_Mat * temp;

					for (int k = 0; k < img->getBand(); ++k)
					{
						pcaimg->setBlock(k, a, b, result(k));
					}

				}
			}

			pcaimg->saveDatabyBlock(i, j);
			img->clear();
			pcaimg->clear();

		}
	}



	img_forward = pcaimg;

	return pcaimg;
}

Image* PCATrans::PCAinverse(string& outpath)
{
	Image* pcaimg = tmanager->create();
	pcaimg->create(img_forward->getBand(), img_forward->getRow(), img_forward->getCol(), img, outpath);

	for (int i = 0; i < img_forward->getBlockRow(); ++i)
	{
		for (int j = 0; j < img_forward->getBlockCol(); ++j)
		{
			img_forward->loadDatabyBlock(i, j);
			pcaimg->prepareDatabyBlock(i, j);

			for (int a = 0; a < img_forward->getEachBlockRow(); ++a)
			{
				for (int b = 0; b < img_forward->getEachBlockCol(); ++b)
				{
					VectorXd temp(img_forward->getBand());

					for (int k = 0; k < img_forward->getBand(); ++k)
					{
						temp(k) = img_forward->atBlock(k, a, b);
					}

					VectorXd result = inverse_Mat * temp;

					for (int k = 0; k < img_forward->getBand(); ++k)
					{
						pcaimg->setBlock(k, a, b, result(k));
					}

				}
			}

			pcaimg->saveDatabyBlock(i, j);
			img->clear();
			pcaimg->clear();

		}
	}


	return pcaimg;
}

bool PCATrans::calTransMat()
{
	if (sta == nullptr) { return false; }

	MatrixXd* cov_mat = sta->coVarMat();
	int band = img->getBand();

	forward_Mat = MatrixXd::Constant(band, band, -1000000);
	inverse_Mat = MatrixXd::Constant(band, band, -1000000);
	MatrixXd etemp(band, band);

	cv::Mat temp(band, band, CV_64FC1);
	cv::Mat thevalue(band, band, CV_64FC1);
	cv::Mat thevector(band, band, CV_64FC1);

	for (int i = 0; i < band; ++i)
	{
		for (int j = 0; j < band; ++j)
		{
			temp.at<double>(i, j) = (*cov_mat)(i, j);
		}
	}

	cv::eigen(temp, thevalue, thevector);


	for (int i = 0; i < band; ++i)
	{
		for (int j = 0; j < band; ++j)
		{
			etemp(i, j) = thevector.at<double>(i, j);
		}
	}

	forward_Mat = etemp.transpose();
	inverse_Mat = forward_Mat.inverse();


	return true;

}
