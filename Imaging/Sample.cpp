#include "StdAfx.h"
#include "Sample.h"
#include <iostream>
#include <cmath>
using namespace std;

CGcpGeoCorrect::CGcpGeoCorrect()
{
	m_roder = 0;
	m_GcpPoint.count = 0;
	m_GcpPoint.e1 = NULL;
	m_GcpPoint.n1 = NULL;
	m_GcpPoint.e2 = NULL;
	m_GcpPoint.n2 = NULL;
	m_GcpPoint.status = NULL;
	m_E12 = new double[20];
	m_N12 = new double[20];
	m_E21 = new double[20];
	m_N21 = new double[20];
}

CGcpGeoCorrect::~CGcpGeoCorrect()
{
	if (m_GcpPoint.e1)
		delete[] m_GcpPoint.e1;
	m_GcpPoint.e1 = NULL;
	if (m_GcpPoint.n1)
		delete[] m_GcpPoint.n1;
	m_GcpPoint.n1 = NULL;
	if (m_GcpPoint.e2)
		delete[] m_GcpPoint.e2;
	m_GcpPoint.e2 = NULL;
	if (m_GcpPoint.n2)
		delete[] m_GcpPoint.n2;
	m_GcpPoint.n2 = NULL;
	if (m_GcpPoint.status)
		delete[] m_GcpPoint.status;
	m_GcpPoint.status = NULL;

	if (m_E12)
		delete[] m_E12;
	m_E12 = NULL;
	if (m_N12)
		delete[] m_N12;
	m_N12 = NULL;
	if (m_E21)
		delete[] m_E21;
	m_E21 = NULL;
	if (m_N21) 
		delete[] m_N21;
	m_N21 = NULL;
}

int CGcpGeoCorrect::LoadGcpPoint(CString filepath)
{
	FILE *pf_gcp = NULL;
	pf_gcp = fopen(filepath, "r");
	if (pf_gcp==NULL)
		return 0;
	fscanf(pf_gcp,"%d\n", &m_GcpPoint.count);
	if (m_GcpPoint.count<3)
		return 0;
	//申请控制点内存
	if (m_GcpPoint.e1)
		delete[] m_GcpPoint.e1;
	m_GcpPoint.e1 = new double[m_GcpPoint.count];
	if (m_GcpPoint.n1)
		delete[] m_GcpPoint.n1;
	m_GcpPoint.n1 = new double[m_GcpPoint.count];
	if (m_GcpPoint.e2)
		delete[] m_GcpPoint.e2;
	m_GcpPoint.e2 = new double[m_GcpPoint.count];
	if (m_GcpPoint.n2)
		delete[] m_GcpPoint.n2;
	m_GcpPoint.n2 = new double[m_GcpPoint.count];
	if (m_GcpPoint.status)
		delete[] m_GcpPoint.status;
	m_GcpPoint.status = new int[m_GcpPoint.count];
	//读入控制点
	char tem[30];
	for(int i = 0; i < m_GcpPoint.count; i++)
	{ 
		fscanf(pf_gcp,"%s ", tem);
		fscanf(pf_gcp,"%lf ", &m_GcpPoint.e1[i]);	//参考影像
		fscanf(pf_gcp,"%lf ", &m_GcpPoint.n1[i]);

		fscanf(pf_gcp,"%lf ", &m_GcpPoint.e2[i]);	//待校正影像
		fscanf(pf_gcp,"%lf\n", &m_GcpPoint.n2[i]);

		m_GcpPoint.status[i] = 1;
	}
	fclose(pf_gcp);
	return 1;
}

void CGcpGeoCorrect::computfacter(int order)
{
	if (order<1||order>3)
		return ;
	m_roder = order;
	CRS_compute_georef_equations(&m_GcpPoint,m_E12,m_N12,m_E21,m_N21,order);
}

void CGcpGeoCorrect::GetBaseCoord(double WarpX,double WarpY,double *BaseX,double *BaseY)
{
	//参考影像坐标 = 系数 * 校正影像坐标  重采样
	CRS_georef(WarpX,WarpY,BaseX,BaseY,m_E21,m_N21,m_roder);
}
void CGcpGeoCorrect::GetWarpCoord(double BaseX,double BaseY,double *WarpX,double *WarpY)
{
	//校正影像坐标 = 系数 * 参考影像坐标  确定范围dstdata.tif
	CRS_georef(BaseX,BaseY,WarpX,WarpY,m_E12,m_N12,m_roder);
}

void CGcpGeoCorrect::Correct(CString infile,CString outfile,int sample)
{
	GDALAllRegister();  // 注册驱动
	GDALDataset * hSrcDS = NULL;
	hSrcDS = (GDALDataset *)GDALOpen(infile,GA_ReadOnly);

	//读取原始数据
	long SrcWidth = hSrcDS->GetRasterXSize();
	long SrcHeight = hSrcDS->GetRasterYSize();
	long SrcRcount = hSrcDS->GetRasterCount();
	char * SrcData = new char[SrcWidth*SrcHeight*SrcRcount];
	hSrcDS->RasterIO(GF_Read,0,0,SrcWidth,SrcHeight,SrcData,SrcWidth,SrcHeight,GDT_Byte,3,0,0,0,0);

	//计算校正后范围
	double x = 0;
	double y = 0;
	double MaxX = 0;
	double MinX = 0;
	double MaxY = 0;
	double MinY = 0;
	for (int i=0;i<SrcWidth;i++)
	{
		GetWarpCoord(i,0,&x,&y);
		if (i==0)
		{
			MinX = x;
			MaxX = x;
			MinY = y;
			MaxY = y;
		}
		if (MinX>x) MinX = x;
		if (MaxX<x) MaxX = x;
		if (MinY>y) MinY = y;
		if (MaxY<y) MaxY = y;
		GetWarpCoord(i,SrcHeight-1,&x,&y);
		if (MinX>x) MinX = x;
		if (MaxX<x) MaxX = x;
		if (MinY>y) MinY = y;
		if (MaxY<y) MaxY = y;
	}
	for (int i=0;i<SrcHeight;i++)
	{
		GetWarpCoord(0,i,&x,&y);
		if (MinX>x) MinX = x;
		if (MaxX<x) MaxX = x;
		if (MinY>y) MinY = y;
		if (MaxY<y) MaxY = y;
		GetWarpCoord(SrcWidth-1,i,&x,&y);
		if (MinX>x) MinX = x;
		if (MaxX<x) MaxX = x;
		if (MinY>y) MinY = y;
		if (MaxY<y) MaxY = y;
	}

	double minx = MinX;	//x偏移量
	double miny = MinY;	//y偏移量
	double temX = MaxX-MinX;	//X大小
	double temY = MaxY-MinY;	//Y大小

	//创建输出影像
	GDALDataset * hDstDS = NULL;	//目的影像
	GDALDriverH hDriver;
	GDALDataType eDT = GDALGetRasterDataType(GDALGetRasterBand(hSrcDS,1));
	hDriver = GDALGetDriverByName( "GTiff" );
	CPLAssert( hDriver != NULL );
	hDstDS = (GDALDataset *)GDALCreate( hDriver, outfile, (long)temX, (long)temY, 
		SrcRcount, eDT, NULL );
	CPLAssert( hDstDS != NULL );
	double adfGeoTransform[6]={0,1,0,0,0,1};
	hDstDS->SetGeoTransform(adfGeoTransform);

	//重采样
	long bandlen = (long)temX*(long)temY;
	char * outdata = new char[bandlen*SrcRcount];
	if (outdata == NULL)
	{
		AfxMessageBox("创建存储影像内存失败！");
		return;
	}
	//开始重采样
	for (long i=0;i<(long)temY;i++)
	{
		for (long j=0;j<(long)temX;j++)
		{
			GetBaseCoord(i+minx,j+miny,&x,&y);
			if (y>0&&y<SrcWidth&&x>0&&x<SrcHeight)
			{	
				if(sample == 1)//最近邻
				{
					long xx = x+0.5;
					if (xx == SrcHeight) xx--;
					long yy = y+0.5;
					if (yy == SrcWidth) yy--;
					long band1 = xx*SrcWidth+yy;
					long jj = i*(long)temX+j;
					for (int k=0;k<SrcRcount;k++)
					{
						outdata[jj+k*bandlen] = SrcData[band1+k*SrcWidth*SrcHeight];						
					}
				}
				if(sample == 2) //双线性内插
				{
					long xx = x;
					long yy = y;
					long jj = i*(long)temX+j;

					long band1 = 0;
					double z_dk2 = 0;
					double _dk2 = 0;
					double Dk2 = 0;
					long tx = 0;
					long ty = 0;
					for (int k=0;k<SrcRcount;k++)
					{
						z_dk2 = 0;
						_dk2 = 0;
						for (long i=xx;i<xx+2;i++)
						{
							for (long j=yy;j<yy+2;j++)
							{
								tx = i;
								ty = j;
								if (i>=SrcHeight) tx = 2*SrcHeight-i-1;
								if (j>=SrcHeight) tx = 2*SrcWidth-j-1;
								band1 = tx*SrcWidth+ty;
								Dk2 = pow((x-i),2)+pow((y-j),2);
								z_dk2 += SrcData[band1+k*SrcWidth*SrcHeight]/Dk2;
								_dk2 += 1/Dk2;				
							}
						}
						outdata[jj+k*bandlen] =(long)z_dk2/_dk2;	
					}
				}

				if(sample == 3)//三次卷积
				{
					long xx = x;
					long yy = y;
					long jj = i*(long)temX+j;

					long band1 = 0;
					double z_dk2 = 0;
					double _dk2 = 0;
					double Dk2 = 0;
					long tx = 0;
					long ty = 0;
					for (int k=0;k<SrcRcount;k++)
					{
						z_dk2 =0;
						_dk2 = 0;
						for (long i=xx-1;i<xx+3;i++)
						{
							for (long j=yy-1;j<yy+3;j++)
							{
								tx = i;
								ty = j;
								if (i<0) tx = abs(i);
								if (i>=SrcHeight) tx = 2*SrcHeight-i-1;
								if (j<0) ty = abs(j);
								if (j>=SrcHeight) tx = 2*SrcWidth-j-1;
								band1 = tx*SrcWidth+ty;
								Dk2 = pow((x-i),2)+pow((y-j),2);
								z_dk2 += SrcData[band1+k*SrcWidth*SrcHeight]/Dk2;
								_dk2 += 1/Dk2;				
							}
						}
						outdata[jj+k*bandlen] =(long)z_dk2/_dk2;					
					}

				}

			}

		}
	}
	hDstDS->RasterIO(GF_Write,0,0,(long)temX,(long)temY,outdata,(long)temX,(long)temY,GDT_Byte,3,0,0,0,0);

	GDALClose(hDstDS);
	GDALClose(hSrcDS);
	AfxMessageBox("几何校正结束！");
}


//============================私有成员函数==========================//
int CGcpGeoCorrect::
CRS_georef (
			double e1,  /* EASTINGS TO BE TRANSFORMED */
			double n1,  /* NORTHINGS TO BE TRANSFORMED */
			double *e,  /* EASTINGS TO BE TRANSFORMED */
			double *n,  /* NORTHINGS TO BE TRANSFORMED */
			double E[], /* EASTING COEFFICIENTS */
			double N[], /* NORTHING COEFFICIENTS */
			int order  /* ORDER OF TRANSFORMATION TO BE PERFORMED, MUST MATCH THE
					   ORDER USED TO CALCULATE THE COEFFICIENTS */
					   )
{
	double e3, e2n, en2, n3, e2, en, n2;

	switch(order)
	{
	case 1:

		*e = E[0] + E[1] * e1 + E[2] * n1;
		*n = N[0] + N[1] * e1 + N[2] * n1;
		break;

	case 2:

		e2 = e1 * e1;
		n2 = n1 * n1;
		en = e1 * n1;

		*e = E[0]      + E[1] * e1 + E[2] * n1 +
			E[3] * e2 + E[4] * en + E[5] * n2;
		*n = N[0]      + N[1] * e1 + N[2] * n1 +
			N[3] * e2 + N[4] * en + N[5] * n2;
		break;

	case 3:

		e2  = e1 * e1;
		en  = e1 * n1;
		n2  = n1 * n1;
		e3  = e1 * e2;
		e2n = e2 * n1;
		en2 = e1 * n2;
		n3  = n1 * n2;

		*e = E[0]      +
			E[1] * e1 + E[2] * n1  +
			E[3] * e2 + E[4] * en  + E[5] * n2  +
			E[6] * e3 + E[7] * e2n + E[8] * en2 + E[9] * n3;
		*n = N[0]      +
			N[1] * e1 + N[2] * n1  +
			N[3] * e2 + N[4] * en  + N[5] * n2  +
			N[6] * e3 + N[7] * e2n + N[8] * en2 + N[9] * n3;
		break;

	default:

		return(MPARMERR);
	}

	return(MSUCCESS);
}

int CGcpGeoCorrect::
CRS_compute_georef_equations (struct Control_Points *cp, 
							  double E12[], double N12[], 
							  double E21[], double N21[], 
							  int order)
{
	double *tempptr;
	int status;

	if(order < 1 || order > MAXORDER)
		return(MPARMERR);

	/* CALCULATE THE FORWARD TRANSFORMATION COEFFICIENTS */

	status = calccoef(cp,E12,N12,order);
	if(status != MSUCCESS)
		return(status);

	/* SWITCH THE 1 AND 2 EASTING AND NORTHING ARRAYS */

	tempptr = cp->e1;
	cp->e1 = cp->e2;
	cp->e2 = tempptr;
	tempptr = cp->n1;
	cp->n1 = cp->n2;
	cp->n2 = tempptr;

	/* CALCULATE THE BACKWARD TRANSFORMATION COEFFICIENTS */

	status = calccoef(cp,E21,N21,order);

	/* SWITCH THE 1 AND 2 EASTING AND NORTHING ARRAYS BACK */

	tempptr = cp->e1;
	cp->e1 = cp->e2;
	cp->e2 = tempptr;
	tempptr = cp->n1;
	cp->n1 = cp->n2;
	cp->n2 = tempptr;

	return(status);
}

/***************************************************************************/
/*
COMPUTE THE GEOREFFERENCING COEFFICIENTS BASED ON A SET OF CONTROL POINTS
*/
/***************************************************************************/

int CGcpGeoCorrect::
calccoef (struct Control_Points *cp, double E[], double N[], int order)
{
	struct MATRIX m;
	double *a;
	double *b;
	int numactive;   /* NUMBER OF ACTIVE CONTROL POINTS */
	int status, i;

	/* CALCULATE THE NUMBER OF VALID CONTROL POINTS */

	for(i = numactive = 0 ; i < cp->count ; i++)
	{
		if(cp->status[i] > 0)
			numactive++;
	}

	/* CALCULATE THE MINIMUM NUMBER OF CONTROL POINTS NEEDED TO DETERMINE
	A TRANSFORMATION OF THIS ORDER */

	m.n = ((order + 1) * (order + 2)) / 2;

	if(numactive < m.n)
		return(MNPTERR);

	/* INITIALIZE MATRIX */

	//m.v = (double *)CPLCalloc(m.n*m.n,sizeof(double));
	m.v = new double[m.n*m.n];
	if(m.v == NULL)
	{
		return(MMEMERR);
	}
	//a = (double *)CPLCalloc(m.n,sizeof(double));
	a = new double[m.n];
	if(a == NULL)
	{
		delete[] ((char *)m.v);
		return(MMEMERR);
	}
	//b = (double *)CPLCalloc(m.n,sizeof(double));
	b = new double[m.n];
	if(b == NULL)
	{
		delete[] ((char *)m.v);
		delete[] ((char *)a);
		return(MMEMERR);
	}

	if(numactive == m.n)
		status = exactdet(cp,&m,a,b,E,N);
	else
		status = calcls(cp,&m,a,b,E,N);

	delete[] ((char *)m.v);
	delete[] ((char *)a);
	delete[] ((char *)b);

	return(status);
}

/***************************************************************************/
/*
CALCULATE THE TRANSFORMATION COEFFICIENTS WITH EXACTLY THE MINIMUM
NUMBER OF CONTROL POINTS REQUIRED FOR THIS TRANSFORMATION.
*/
/***************************************************************************/

int CGcpGeoCorrect::exactdet (
struct Control_Points *cp,
struct MATRIX *m,
	double a[],
	double b[],
	double E[],     /* EASTING COEFFICIENTS */
	double N[]     /* NORTHING COEFFICIENTS */
)
{
	int pntnow, currow, j;

	currow = 1;
	for(pntnow = 0 ; pntnow < cp->count ; pntnow++)
	{
		if(cp->status[pntnow] > 0)
		{
			/* POPULATE MATRIX M */

			for(j = 1 ; j <= m->n ; j++)
			{
				M(currow,j) = term(j,cp->e1[pntnow],cp->n1[pntnow]);
			}

			/* POPULATE MATRIX A AND B */

			a[currow-1] = cp->e2[pntnow];
			b[currow-1] = cp->n2[pntnow];

			currow++;
		}
	}

	if(currow - 1 != m->n)
		return(MINTERR);

	return(solvemat(m,a,b,E,N));
}

/***************************************************************************/
/*
CALCULATE THE TRANSFORMATION COEFFICIENTS WITH MORE THAN THE MINIMUM
NUMBER OF CONTROL POINTS REQUIRED FOR THIS TRANSFORMATION.  THIS
ROUTINE USES THE LEAST SQUARES METHOD TO COMPUTE THE COEFFICIENTS.
*/
/***************************************************************************/

int CGcpGeoCorrect::calcls (
struct Control_Points *cp,
struct MATRIX *m,
	double a[],
	double b[],
	double E[],     /* EASTING COEFFICIENTS */
	double N[]     /* NORTHING COEFFICIENTS */
)
{
	int i, j, n, numactive = 0;

	/* INITIALIZE THE UPPER HALF OF THE MATRIX AND THE TWO COLUMN VECTORS */

	for(i = 1 ; i <= m->n ; i++)
	{
		for(j = i ; j <= m->n ; j++)
			M(i,j) = 0.0;
		a[i-1] = b[i-1] = 0.0;
	}

	/* SUM THE UPPER HALF OF THE MATRIX AND THE COLUMN VECTORS ACCORDING TO
	THE LEAST SQUARES METHOD OF SOLVING OVER DETERMINED SYSTEMS */

	for(n = 0 ; n < cp->count ; n++)
	{
		if(cp->status[n] > 0)
		{
			numactive++;
			for(i = 1 ; i <= m->n ; i++)
			{
				for(j = i ; j <= m->n ; j++)
					M(i,j) += term(i,cp->e1[n],cp->n1[n]) * term(j,cp->e1[n],cp->n1[n]);

				a[i-1] += cp->e2[n] * term(i,cp->e1[n],cp->n1[n]);
				b[i-1] += cp->n2[n] * term(i,cp->e1[n],cp->n1[n]);
			}
		}
	}

	if(numactive <= m->n)
		return(MINTERR);

	/* TRANSPOSE VALUES IN UPPER HALF OF M TO OTHER HALF */

	for(i = 2 ; i <= m->n ; i++)
	{
		for(j = 1 ; j < i ; j++)
			M(i,j) = M(j,i);
	}

	return(solvemat(m,a,b,E,N));
}

/***************************************************************************/
/*
CALCULATE THE X/Y TERM BASED ON THE TERM NUMBER

ORDER\TERM   1    2    3    4    5    6    7    8    9   10
1        e0n0 e1n0 e0n1
2        e0n0 e1n0 e0n1 e2n0 e1n1 e0n2
3        e0n0 e1n0 e0n1 e2n0 e1n1 e0n2 e3n0 e2n1 e1n2 e0n3
*/
/***************************************************************************/

double CGcpGeoCorrect::term (int term, double e, double n)
{
	switch(term)
	{
	case  1: return((double)1.0);
	case  2: return((double)e);
	case  3: return((double)n);
	case  4: return((double)(e*e));
	case  5: return((double)(e*n));
	case  6: return((double)(n*n));
	case  7: return((double)(e*e*e));
	case  8: return((double)(e*e*n));
	case  9: return((double)(e*n*n));
	case 10: return((double)(n*n*n));
	}
	return((double)0.0);
}

/***************************************************************************/
/*
SOLVE FOR THE 'E' AND 'N' COEFFICIENTS BY USING A SOMEWHAT MODIFIED
GAUSSIAN ELIMINATION METHOD.

| M11 M12 ... M1n | | E0   |   | a0   |
| M21 M22 ... M2n | | E1   | = | a1   |
|  .   .   .   .  | | .    |   | .    |
| Mn1 Mn2 ... Mnn | | En-1 |   | an-1 |

and

| M11 M12 ... M1n | | N0   |   | b0   |
| M21 M22 ... M2n | | N1   | = | b1   |
|  .   .   .   .  | | .    |   | .    |
| Mn1 Mn2 ... Mnn | | Nn-1 |   | bn-1 |
*/
/***************************************************************************/

int CGcpGeoCorrect::solvemat (struct MATRIX *m,
					 double a[], double b[], double E[], double N[])
{
	int i, j, i2, j2, imark;
	double factor, temp;
	double  pivot;  /* ACTUAL VALUE OF THE LARGEST PIVOT CANDIDATE */

	for(i = 1 ; i <= m->n ; i++)
	{
		j = i;

		/* find row with largest magnitude value for pivot value */

		pivot = M(i,j);
		imark = i;
		for(i2 = i + 1 ; i2 <= m->n ; i2++)
		{
			temp = fabs(M(i2,j));
			if(temp > fabs(pivot))
			{
				pivot = M(i2,j);
				imark = i2;
			}
		}

		/* if the pivot is very small then the points are nearly co-linear */
		/* co-linear points result in an undefined matrix, and nearly */
		/* co-linear points results in a solution with rounding error */

		if(pivot == 0.0)
			return(MUNSOLVABLE);

		/* if row with highest pivot is not the current row, switch them */

		if(imark != i)
		{
			for(j2 = 1 ; j2 <= m->n ; j2++)
			{
				temp = M(imark,j2);
				M(imark,j2) = M(i,j2);
				M(i,j2) = temp;
			}

			temp = a[imark-1];
			a[imark-1] = a[i-1];
			a[i-1] = temp;

			temp = b[imark-1];
			b[imark-1] = b[i-1];
			b[i-1] = temp;
		}

		/* compute zeros above and below the pivot, and compute
		values for the rest of the row as well */

		for(i2 = 1 ; i2 <= m->n ; i2++)
		{
			if(i2 != i)
			{
				factor = M(i2,j) / pivot;
				for(j2 = j ; j2 <= m->n ; j2++)
					M(i2,j2) -= factor * M(i,j2);
				a[i2-1] -= factor * a[i-1];
				b[i2-1] -= factor * b[i-1];
			}
		}
	}

	/* SINCE ALL OTHER VALUES IN THE MATRIX ARE ZERO NOW, CALCULATE THE
	COEFFICIENTS BY DIVIDING THE COLUMN VECTORS BY THE DIAGONAL VALUES. */

	for(i = 1 ; i <= m->n ; i++)
	{
		E[i-1] = a[i-1] / M(i,i);
		N[i-1] = b[i-1] / M(i,i);
	}

	return(MSUCCESS);
}