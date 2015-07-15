#include "stdafx.h"
#include "UpDt.h"
#include <vector>
#include <fstream>
using namespace std;

UpDt::UpDt()
{
	m_iclass = 0;
}

void UpDt::ReadFile(CString first, CString second)
{
	GDALAllRegister();        //注册驱动
	GDALDataset *poDataset1; //gdal数据集

	poDataset1 = (GDALDataset *)GDALOpen(first, GA_ReadOnly );   //打开数据
	if( poDataset1 == NULL )
	{
		AfxMessageBox("第一幅影像文件打开失败!!!");
		return;
	} 
	m_nxsize = poDataset1->GetRasterXSize();
	m_nysize = poDataset1->GetRasterYSize();

	GDALDataset *poDataset2; //gdal数据集

	poDataset2 = (GDALDataset *)GDALOpen(second, GA_ReadOnly );   //打开数据
	if( poDataset2 == NULL )
	{
		AfxMessageBox("第二幅影像文件打开失败!!!");
		return;
	} 
	
	int xsize,ysize;
	xsize = poDataset2->GetRasterXSize();
	ysize = poDataset2->GetRasterYSize();

	if (m_nxsize != xsize || m_nysize != ysize)
	{
		AfxMessageBox("两幅影像不是一样大");
		return;
	}

	m_first = (BYTE *) CPLMalloc(sizeof(BYTE)*(m_nxsize)*(m_nysize));
	GDALRasterBand *poBand1;
	poBand1 = poDataset1->GetRasterBand(1);
	poBand1->RasterIO( GF_Read, 0,0,m_nxsize,m_nysize, 
		m_first,m_nxsize,m_nysize, GDT_Byte,0, 0);

	m_second = (BYTE *) CPLMalloc(sizeof(BYTE)*(m_nxsize)*(m_nysize));
	GDALRasterBand *poBand2;
	poBand2 = poDataset2->GetRasterBand(1);
	poBand2->RasterIO( GF_Read, 0,0,m_nxsize,m_nysize, 
		m_second,m_nxsize,m_nysize, GDT_Byte,0, 0);

}

void UpDt::Sub(CString outfile)
{
	//首先遍历图像判断有多少类别
	int i,j;
	int it;
	vector <BYTE> ver;
	bool Isfind;
	ver.push_back(m_first[0]);
	m_iclass = 1;
	for(i = 1; i < m_nxsize*m_nysize; i++)
	{
		for (it = 0; it != ver.size(); it++)
		{
			if(m_first[i] == ver[it])
				break;
		}
		 if(it == ver.size()) 
		 {
			m_iclass++;
			ver.push_back(m_first[i]);
		}
	}

	int temp;
	for (j = 1;j < m_iclass;j++)//进行几次判断
	{
		for (i = 0;i <= m_iclass-1-j; i++)//进行互换位置
		{
			if (ver[i] > ver[i+1])
			{
				temp = ver[i];
				ver[i] = ver[i+1];
				ver[i+1] = temp;
			}
		}
	}


	int k;
	int *Matrix = new int[m_iclass*m_iclass]; 
	for (i = 0; i < m_iclass*m_iclass; i++)
	{
		Matrix[i] = 0;
	}

	for (i = 0; i < m_nysize*m_nxsize; i++)
	{
		for (j = 0; j < m_iclass;j++)
		{
			if (m_first[i] == ver[j])
				break;
		}
		for (k = 0; k < m_iclass; k++)
		{
			if (m_second[i] == ver[k])
			break;
		}

		Matrix[k*m_iclass+j]++;
	}

	ofstream m_outfile;
	m_outfile.open(outfile);

	m_outfile<<" 行值为原来的类数信息，列数是现在类数信息！"<<endl;

	for (i = 0; i < m_iclass;i++)
	{
		for (j = 0; j < m_iclass;j++)
		{
			m_outfile<<Matrix[m_iclass*i+j]<<" ";
		}
		m_outfile<<endl;
	}
	m_outfile.close();
	delete []Matrix;
	AfxMessageBox("分类完成！");
}
