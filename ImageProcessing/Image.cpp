#include "Image.h"

Image::Image(Image& other)
{
	this->path = other.path;
	this->file_name = other.file_name;
	this->file_path = other.file_path;

	GDALAllRegister();
	poDataset = (GDALDataset*)GDALOpen(path, GA_ReadOnly);

	col = poDataset->GetRasterXSize();
	row = poDataset->GetRasterYSize();
	band = poDataset->GetRasterCount();

	data.resize(band);
	fdata.resize(band);
	for (int i = 0; i < band; ++i)
	{
		data[i] = nullptr;
		fdata[i] = nullptr;
	}


}

Image::~Image()
{
	if (poDataset != nullptr)
	{
		GDALClose(poDataset);
	}

	for (int i = 0; i < data.size(); ++i)
	{
		if (data[i] != nullptr)
		{
			delete data[i];
		}
	}
}

bool Image::getPath(string& thepath)
{
	file_path = thepath;
	path = thepath.c_str();


	GDALAllRegister();
	poDataset = (GDALDataset*)GDALOpen(path, GA_ReadOnly);

	col = poDataset->GetRasterXSize();
	row = poDataset->GetRasterYSize();
	band = poDataset->GetRasterCount();

	poDataset->GetGeoTransform(geotrans);

	GDALRasterBand* poBand = poDataset->GetRasterBand(1);
	GDALDataType thetype = poBand->GetRasterDataType();

	if (thetype == GDT_Byte)isbyte = true;
	else isbyte = false;

	if (thetype == GDT_Float32 || thetype == GDT_Float64)
	{
		isfloat = true;
		fdata.resize(band);
		for (int i = 0; i < band; ++i)
		{
			fdata[i] = nullptr;
		}
	}
	else
	{
		isfloat = false;
		data.resize(band);

		for (int i = 0; i < band; ++i)
		{
			data[i] = nullptr;
		}
	}

	b_row = row / block_row + 1;
	b_col = col / block_col + 1;

	return true;
}

bool Image::loadData(int band_1)
{


	if (band_1 != -1)
	{
		GDALRasterBand* poBand = poDataset->GetRasterBand(band_1 + 1);

		if (isfloat)
		{
			if (fdata[band_1] == nullptr)
			{
				fdata[band_1] = new float[row * col];
			}

			poBand->RasterIO(GF_Read, 0, 0, col, row, fdata[band_1], col, row, GDT_Float32, 0, 0);
		}
		else
		{
			if (data[band_1] == nullptr)
			{
				data[band_1] = new unsigned short[row * col];
			}

			poBand->RasterIO(GF_Read, 0, 0, col, row, data[band_1], col, row, GDT_UInt16, 0, 0);
		}



	}
	else
	{
		if (isfloat)
		{
			for (int i = 0; i < band; ++i)
			{
				if (fdata[i] == nullptr)
				{
					fdata[i] = new float[row * col];
				}
				GDALRasterBand* poBand = poDataset->GetRasterBand(i + 1);
				poBand->RasterIO(GF_Read, 0, 0, col, row, data[i], col, row, GDT_Float32, 0, 0);
			}
		}
		else
		{
			for (int i = 0; i < band; ++i)
			{
				if (data[i] == nullptr)
				{
					data[i] = new unsigned short[row * col];
				}
				GDALRasterBand* poBand = poDataset->GetRasterBand(i + 1);
				poBand->RasterIO(GF_Read, 0, 0, col, row, data[i], col, row, GDT_UInt16, 0, 0);
			}
		}

	}


	return true;
}

bool Image::loadDatabyBlock(int brow, int bcol, int band_1)
{
	currentblock_col = block_col;
	currentblock_row = block_row;

	if (col - bcol * block_col < block_col) { currentblock_col = col - bcol * block_col; }
	if (row - brow * block_row < block_row) { currentblock_row = row - brow * block_row; }


	if (band_1 != -1)
	{
		GDALRasterBand* poBand = poDataset->GetRasterBand(band_1 + 1);

		if (isfloat)
		{
			if (fdata[band_1] == nullptr)
			{
				fdata[band_1] = new float[currentblock_row * currentblock_col];
			}

			poBand->RasterIO(GF_Read, bcol * block_col, brow * block_row, currentblock_col, currentblock_row, fdata[band_1], currentblock_col, currentblock_row, GDT_Float32, 0, 0);
		}
		else
		{
			if (data[band_1] == nullptr)
			{
				data[band_1] = new unsigned short[currentblock_row * currentblock_col];
			}

			poBand->RasterIO(GF_Read, bcol * block_col, brow * block_row, currentblock_col, currentblock_row, data[band_1], currentblock_col, currentblock_row, GDT_UInt16, 0, 0);
		}
	}
	else
	{
		if (isfloat)
		{
			for (int i = 0; i < band; ++i)
			{
				if (fdata[i] == nullptr)
				{
					fdata[i] = new float[currentblock_row * currentblock_col];
				}
				GDALRasterBand* poBand = poDataset->GetRasterBand(i + 1);
				poBand->RasterIO(GF_Read, bcol * block_col, brow * block_row, currentblock_col, currentblock_row, fdata[i], currentblock_col, currentblock_row, GDT_Float32, 0, 0);
			}
		}
		else
		{
			for (int i = 0; i < band; ++i)
			{
				if (data[i] == nullptr)
				{
					data[i] = new unsigned short[currentblock_row * currentblock_col];
				}
				GDALRasterBand* poBand = poDataset->GetRasterBand(i + 1);
				poBand->RasterIO(GF_Read, bcol * block_col, brow * block_row, currentblock_col, currentblock_row, data[i], currentblock_col, currentblock_row, GDT_UInt16, 0, 0);
			}
		}

	}


	return true;
}

bool Image::saveData(int band_1)
{

	if (band_1 == -1)
	{
		if (isfloat)
		{
			for (int i = 0; i < band; ++i)
			{
				GDALRasterBand* poBand;
				poBand = poDataset->GetRasterBand(i + 1);
				poBand->RasterIO(GF_Write, 0, 0, col, row, fdata[i], col, row, GDT_Float32, 0, 0);
			}
		}
		else
		{
			for (int i = 0; i < band; ++i)
			{
				GDALRasterBand* poBand;
				poBand = poDataset->GetRasterBand(i + 1);
				poBand->RasterIO(GF_Write, 0, 0, col, row, data[i], col, row, GDT_UInt16, 0, 0);
			}
		}
	}
	else
	{
		if (isfloat)
		{
			GDALRasterBand* poBand;
			poBand = poDataset->GetRasterBand(band_1 + 1);
			poBand->RasterIO(GF_Write, 0, 0, col, row, fdata[band_1], col, row, GDT_Float32, 0, 0);
		}
		else
		{
			GDALRasterBand* poBand;
			poBand = poDataset->GetRasterBand(band_1 + 1);
			poBand->RasterIO(GF_Write, 0, 0, col, row, data[band_1], col, row, GDT_UInt16, 0, 0);
		}
	}

	//GDALClose((GDALDatasetH)poDataset);
	this->clear();

	return true;
}

bool Image::create(int theband, int therow, int thecol, Image* raw, string& opath, bool isthefloat)
{
	if (poDataset != nullptr) { return false; }

	origin = raw;
	band = theband;
	col = thecol;
	row = therow;
	file_path = opath;

	isfloat = isthefloat;

	if (isfloat)
	{
		fdata.resize(band);
		for (int i = 0; i < band; ++i)
		{
			fdata[i] = nullptr;
		}
	}
	else
	{
		data.resize(band);

		for (int i = 0; i < band; ++i)
		{
			data[i] = nullptr;
		}
	}

	const char* outpath = file_path.c_str();

	GDALDriver* poDriver;

	//一般使用tif作为输出
	const char* pszFormat = "GTiff";
	poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);//获取特殊的驱动。
	if (poDriver == NULL) {
		return false;
	}

	char** papszOptions = NULL;
	papszOptions = CSLSetNameValue(papszOptions, "TILED", "YES");
	papszOptions = CSLSetNameValue(papszOptions, "COMPRESS", "PACKBITS");

	//Get size from input Raster

	//Set output Dataset
	//GDALDataset* posaveDataset;
	GDALDataset* originDataset = origin->getDataset();

	if (isfloat)
	{
		poDataset = poDriver->Create(outpath, col, row, band, GDT_Float32, papszOptions);

		//Get the geotrans from the input geotrans

		poDataset->SetGeoTransform(origin->geotrans);
		poDataset->SetProjection(originDataset->GetProjectionRef());

	}
	else
	{
		poDataset = poDriver->Create(outpath, col, row, band, GDT_UInt16, papszOptions);

		//Get the geotrans from the input geotrans

		poDataset->SetGeoTransform(origin->geotrans);
		poDataset->SetProjection(originDataset->GetProjectionRef());
	}

	block_row = 512;
	block_col = 512;

	b_row = row / block_row + 1;
	b_col = col / block_col + 1;

	return true;
}


bool Image::prepareData(int band_1)
{
	if (band_1 == -1)
	{
		if (isfloat)
		{
			for (int i = 0; i < band; ++i)
			{
				fdata[i] = new float[row * col];
			}
		}
		else
		{
			for (int i = 0; i < band; ++i)
			{
				data[i] = new unsigned short[row * col];
			}
		}
	}
	else
	{
		if (isfloat)
		{
			fdata[band_1] = new float[row * col];
		}
		else
		{
			data[band_1] = new unsigned short[row * col];
		}
	}

	return true;
}

bool Image::saveDatabyBlock(int brow, int bcol, int band_1)
{
	currentblock_col = block_col;
	currentblock_row = block_row;

	if (col - bcol * block_col < block_col) { currentblock_col = col - bcol * block_col; }
	if (row - brow * block_row < block_row) { currentblock_row = row - brow * block_row; }

	if (band_1 != -1)
	{
		GDALRasterBand* poBand = poDataset->GetRasterBand(band_1 + 1);

		if (isfloat)
		{
			poBand->RasterIO(GF_Write, bcol * block_col, brow * block_row, currentblock_col, currentblock_row, fdata[band_1], currentblock_col, currentblock_row, GDT_Float32, 0, 0);
		}
		else
		{
			poBand->RasterIO(GF_Write, bcol * block_col, brow * block_row, currentblock_col, currentblock_row, data[band_1], currentblock_col, currentblock_row, GDT_UInt16, 0, 0);
		}
	}
	else
	{
		if (isfloat)
		{
			for (int i = 0; i < band; ++i)
			{
				GDALRasterBand* poBand = poDataset->GetRasterBand(i + 1);
				poBand->RasterIO(GF_Write, bcol * block_col, brow * block_row, currentblock_col, currentblock_row, fdata[i], currentblock_col, currentblock_row, GDT_Float32, 0, 0);
			}
		}
		else
		{
			for (int i = 0; i < band; ++i)
			{
				GDALRasterBand* poBand = poDataset->GetRasterBand(i + 1);
				poBand->RasterIO(GF_Write, bcol * block_col, brow * block_row, currentblock_col, currentblock_row, data[i], currentblock_col, currentblock_row, GDT_UInt16, 0, 0);
			}
		}

	}
	this->clear();
	return false;
}

bool Image::prepareDatabyBlock(int brow, int bcol, int band_1)
{
	currentblock_col = block_col;
	currentblock_row = block_row;

	if (col - bcol * block_col < block_col) { currentblock_col = col - bcol * block_col; }
	if (row - brow * block_row < block_row) { currentblock_row = row - brow * block_row; }

	if (band_1 == -1)
	{
		if (isfloat)
		{
			for (int i = 0; i < band; ++i)
			{
				fdata[i] = new float[currentblock_row * currentblock_col];
			}
		}
		else
		{
			for (int i = 0; i < band; ++i)
			{
				data[i] = new unsigned short[currentblock_row * currentblock_col];
			}
		}
	}
	else
	{
		if (isfloat)
		{
			fdata[band_1] = new float[currentblock_row * currentblock_col];
		}
		else
		{
			data[band_1] = new unsigned short[currentblock_row * currentblock_col];
		}
	}


	return true;
}



float Image::at(int theband, int therow, int thecol)
{
	if (isfloat)
	{
		return fdata[theband][col * therow + thecol];
	}
	else
	{
		return data[theband][col * therow + thecol];
	}

}

float Image::atBlock(int theband, int therow, int thecol)
{
	if (isfloat)
	{
		return fdata[theband][currentblock_col * therow + thecol];
	}
	else
	{
		return data[theband][currentblock_col * therow + thecol];
	}
}


bool Image::set(int theband, int therow, int thecol, float value)
{
	if (isfloat)
	{
		fdata[theband][col * therow + thecol] = value;
	}
	else
	{
		data[theband][col * therow + thecol] = value;
	}

	return true;
}


bool Image::setBlock(int theband, int therow, int thecol, float value)
{
	if (isfloat)
	{
		fdata[theband][currentblock_col * therow + thecol] = value;
	}
	else
	{
		data[theband][currentblock_col * therow + thecol] = value;
	}

	return true;
}

bool Image::clear(int theband)
{
	if (theband == -1)
	{
		for (int i = 0; i < band; ++i)
		{
			if (isfloat)
			{
				if (fdata[i] != nullptr)
				{
					delete fdata[i];
					fdata[i] = nullptr;
				}
			}
			else
			{
				if (data[i] != nullptr)
				{
					delete data[i];
					data[i] = nullptr;
				}
			}
		}
	}
	else
	{
		if (isfloat)
		{
			if (fdata[theband] != nullptr)
			{
				delete fdata[theband];
				fdata[theband] = nullptr;
			}
		}
		else
		{

			if (data[theband] != nullptr)
			{
				delete data[theband];
				data[theband] = nullptr;
			}
		}

	}

	return true;
}

string& Image::getfilepath()
{
	return file_path;
}

string& Image::getName()
{
	string name;
	name = file_path.substr(file_path.rfind('/') + 1);
	file_name = name;
	return file_name;
}