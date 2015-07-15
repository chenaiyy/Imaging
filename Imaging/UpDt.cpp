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
	GDALAllRegister();        //ע������
	GDALDataset *poDataset1; //gdal���ݼ�

	poDataset1 = (GDALDataset *)GDALOpen(first, GA_ReadOnly );   //������
	if( poDataset1 == NULL )
	{
		AfxMessageBox("��һ��Ӱ���ļ���ʧ��!!!");
		return;
	} 
	m_nxsize = poDataset1->GetRasterXSize();
	m_nysize = poDataset1->GetRasterYSize();

	GDALDataset *poDataset2; //gdal���ݼ�

	poDataset2 = (GDALDataset *)GDALOpen(second, GA_ReadOnly );   //������
	if( poDataset2 == NULL )
	{
		AfxMessageBox("�ڶ���Ӱ���ļ���ʧ��!!!");
		return;
	} 
	
	int xsize,ysize;
	xsize = poDataset2->GetRasterXSize();
	ysize = poDataset2->GetRasterYSize();

	if (m_nxsize != xsize || m_nysize != ysize)
	{
		AfxMessageBox("����Ӱ����һ����");
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
	//���ȱ���ͼ���ж��ж������
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
	for (j = 1;j < m_iclass;j++)//���м����ж�
	{
		for (i = 0;i <= m_iclass-1-j; i++)//���л���λ��
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

	m_outfile<<" ��ֵΪԭ����������Ϣ������������������Ϣ��"<<endl;

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
	AfxMessageBox("������ɣ�");
}
