#include "StdAfx.h"
#include "Get_Data.h"

Get_data::Get_data()
{
	GDALAllRegister();        //注册驱动
	poDataset = NULL;	
}

Get_data::~Get_data()
{
	//if (poDataset)
	//{
	//	GDALClose(poDataset);
	//	//delete poDataset;
	//}
	//poDataset = NULL;
}

bool Get_data::ReadFile(CString strFilePath)
{
	poDataset = (GDALDataset *)GDALOpen(strFilePath, GA_ReadOnly );   //打开数据
	if( poDataset == NULL )
	{
		AfxMessageBox("文件打开失败!!!");
		return false;
	} 
	nXsize = poDataset->GetRasterXSize();
	nYsize = poDataset->GetRasterYSize();
	nBands = poDataset->GetRasterCount();
	return true;
}

void Get_data::read1band(BYTE *gray, int band, int xx, int yy)
{
	GDALRasterBand *poBand;
	poBand=poDataset->GetRasterBand(band);
	poBand->RasterIO( GF_Read, 0,0,nXsize,nYsize, 
		gray,xx,yy, GDT_Byte,0, 0);
}

void Get_data::read3band(BYTE *clr,int *index,int xx, int yy)
{
	int Scanline=(nXsize*8+31)/32*4;
	poDataset->RasterIO(GF_Read,0,0,nXsize,nYsize,
		clr,nXsize,nYsize,GDT_Byte,3,index,0,0,0);  //BIP的显示方式
}

void Get_data::readallband(BYTE *pafScan)
{
	poDataset->RasterIO( GF_Read, 0, 0,nXsize,nYsize, pafScan,
		nXsize,nYsize,GDT_Byte,nBands,0,0,0,0);
}