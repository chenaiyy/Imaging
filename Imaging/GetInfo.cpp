#include "stdafx.h"
#include "GetInfo.h"
#include <cmath>

void GetInfo::Histogram(BYTE *data,float *result) //直方图信息
{
	//循环变量
	int i;
	int j;
	//临时变量
	int temp = 0;
	int rt[256];
	for(i = 0; i < 256;i++)
		rt[i] = 0;

	for (i = 0; i < m_ysize; i++)
	{
		for (j = 0; j < m_xsize; j++)
		{
			temp = (int)data[i*m_xsize + j];
			rt[temp] ++;
		}
	}
		for(i = 0;i < 255; i++)
			result[i] = rt[i]/(m_xsize*m_ysize*1.0f);
	
}

void GetInfo::Average(BYTE *data, float &avr) //平均值
{
	long total = 0;
	for (int i = 0; i < m_ysize; i++)
	{
		for (int j = 0; j < m_xsize; j++)
		{
			total += (int)data[i*m_xsize + j];
		}
	}
	avr = total/(m_xsize*m_ysize*1.0f);
}

void GetInfo::Mod(BYTE *data,int& mode)  //取众数
{
	int i;
	int temp;
	int pxl[256];

	for (i = 0; i < 256; i++)
		pxl[i] = 0;

	for (i = 0; i < m_xsize*m_ysize; i++)
	{
		temp = data[i];
		pxl[temp] ++;
	}

	int m = 0;

	for (i = 0; i < 256; i++)
	{
		if(m < pxl[i])
		{
			m = pxl[i];
			mode = i;
		}
	}

}

void GetInfo::Variance(BYTE *data, float &vr)  //方差
{
	float avr;  //取出平均值
	Average(data,avr);

	float total = 0.0f;
	double temp = 0;

	for (int i = 0; i < m_xsize*m_ysize; i++)
	{
		temp = abs(data[i] - avr);	
		total += pow(temp,2);
	}
	vr = total/(m_xsize*m_ysize*1.0f);
}

void GetInfo::Max(BYTE *data, int &max)
{
	int temp = 0; //中间变量
	for (int i = 0; i < m_xsize*m_ysize; i++)
	{
		if (temp < data[i])
		{
			temp = data[i];
		}
	}

	max = temp;
}

void GetInfo::Min(BYTE *data, int &min)
{
	int temp = 255; //中间变量
	for (int i = 0; i < m_xsize*m_ysize; i++)
	{
		if (temp > data[i])
		{
			temp = data[i];
		}
	}

	min = temp;
}