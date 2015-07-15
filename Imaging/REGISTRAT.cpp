#include "stdafx.h"
#include "REGISTRAT.h"
#include <fstream>
#include <cmath>
using namespace std;

struct Point 
{
	double x;
	double y;
};

bool Registrat::ReadGCPS()
{

	char szFilter[] = "文本文件 (*.txt)|*.txt|All Files (*.*)|*.*||";
	CString filePath("");
	CFileDialog fileOpenDlg(TRUE, "txt", NULL,OFN_HIDEREADONLY,szFilter);
	if (fileOpenDlg.DoModal() == IDOK) 
	{
		VERIFY(filePath = fileOpenDlg.GetPathName());
		m_filepath = filePath;
		return true;
	}
	else
		return false;	
}

void Registrat::Getcoef()
{	
	//读取控制点文件
	if(!ReadGCPS())
		return;

	ifstream infile;
	Point Gcps[3][2]; //存储取控制点对
	//循环变量
	int i;
	int j;
	char ch[8];//读取数据时用到

	infile.open(m_filepath); //打开文件
	if(!infile)
	{
		AfxMessageBox("文件打开失败！");
			return;
	}

//读取控制点
	for (i = 0; i < 3; i++)  //0代表参考的，1代表待校正的
	{
		infile.read(ch,7);
		infile>>Gcps[i][0].x;
		infile.read(ch,1);
		infile>>Gcps[i][0].y;
		infile.read(ch,3);
		infile>>Gcps[i][1].x;
		infile.read(ch,1);
		infile>>Gcps[i][1].y;
		infile.read(ch,2);
	}
// 计算相关函数的系数
		double *Matrix = new double[36];
		for (i = 0; i < 36; i ++)
			Matrix[i] = 0;

		Matrix[0] = Gcps[0][0].x;
		Matrix[1] = Gcps[0][0].y;
		Matrix[2] = 1;
		Matrix[9] = Gcps[0][0].x;
		Matrix[10] = Gcps[0][0].y;
		Matrix[11] = 1;
		Matrix[12] = Gcps[1][0].x;
		Matrix[13] = Gcps[1][0].y;
		Matrix[14] = 1;
		Matrix[21] = Gcps[1][0].x;
		Matrix[22] = Gcps[1][0].y;
		Matrix[23] = 1;
		Matrix[24] = Gcps[2][0].x;
		Matrix[25] = Gcps[2][0].y;
		Matrix[26] = 1;		
		Matrix[33] = Gcps[2][0].x;
		Matrix[34] = Gcps[2][0].y;
		Matrix[35] = 1;

		double *C = new double[6];
		for(i = 0; i < 3; i++)
		{
			C[2*i] = Gcps[i][1].x;
			C[2*i+1] = Gcps[i][1].y;
		}
	
		double *xishu = new double[6];

		for (i = 0; i < 6; i++)
		{
			xishu[i] = 0;
		}

		double *Matrix_reve = new double[6*6];
		for (i = 0; i < 6; i++)
			for (j = 0; j < 6; j++)
				Matrix_reve[i*6+j] = 0;

		unitmatrix(Matrix_reve,6);//将Matrix_reve初始化为单位阵
		calculate(Matrix,Matrix_reve,6);//将Matrix化为了上三角阵
		backcalculate(Matrix,Matrix_reve,6);  //将Matrix化为了单位阵，这时Matrix_reve即为所求
		matrix_multiply(Matrix_reve,C,xishu,6,6,1);

		m_a1 = xishu[0];
		m_b1 = xishu[1];
		m_c1 = xishu[2];
		m_a2 = xishu[3];
		m_b2 = xishu[4];
		m_c2 = xishu[5];

}

void Registrat::PreGcps(int x, int y, int &x1, int &y1)
{
	x1 = (int)(m_a1*x + m_b1*y + m_c1 + 0.5);
	y1 = (int)(m_a2*x + m_b2*y + m_c2 + 0.5);
}


void Registrat::swap(double *a,int i,int line,int n) 
// exchange line//交换行位置，i控制行号，line也是行号，//n是矩阵列数
{
	int j;
	double temp;
	for(j=0;j<n;j++)
	{
		temp=a[i*n+j];
		a[i*n+j]=a[line*n+j];
		a[line*n+j]=temp;
	}
}


void Registrat::calculate(double *p,double *q,int n) //形成上三角阵
{
	int i,j,k,m,line;
	double max,temp,mmul;
	for(i=0;i<n;i++)
	{
		max=fabs(p[i*n+i]);
		temp=p[i*n+i];
		line=i;
		for(j=i+1;j<n;j++)       
			//选出每一列中最大值，并且用line记录行号,
			//用temp记录该最大值，用j来控制行号
		{
			if(fabs(p[j*n+i])>max)
			{
				max=fabs(p[j*n+i]); 
				temp=p[j*n+i]; 
				line=j;
			}
		}  
		if(max<=1e-10)  
		{
			AfxMessageBox("读取的控制点无法求解出对应的相关函数系数");
			return;
		}
		if(line!=i)
		{ 
			swap(p,i,line,n);//将每一列中最大行换到i行
			swap(q,i,line,n);
		}
		for(k=0;k<n;k++)
		{
			p[i*n+k]/=temp;//将i行的每个数都除以i行i列的值，将i行i列化为1
			q[i*n+k]/=temp;
		}
		for(k=i+1;k<n;k++)//将i列i行下面其它行的值都化为0
		{
			mmul=p[k*n+i];
			for(m=0;m<n;m++)
			{
				p[k*n+m]-=p[i*n+m]*mmul;//每一行都减去Ak0*i行对应列的值
				q[k*n+m]-=q[i*n+m]*mmul;
			}
		}
	} 

}

void Registrat::backcalculate(double*p,double*q,int n)//形成单位矩阵  
{
	int i,j,k;
	double mmul;
	for(i=n-1;i>0;i--)
	{
		for(j=i-1;j>=0;j--)//从下往上每一行进行计算，与前面相反
		{
			mmul=p[j*n+i];
			p[j*n+i]-=p[i*n+i]*mmul;
			for(k=0;k<n;k++)
			{
				q[j*n+k]-=q[i*n+k]*mmul;
			}
		}
	}

}
void Registrat::unitmatrix(double *q,int n)   //形成单位矩阵，矩阵q是一个单位阵
{
	int i,j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			if(i==j)
			{
				q[i*n+j]=1;
			}
			else
				q[i*n+j]=0;
		}
	}
}

void Registrat::matrix_multiply(double *A,double *B,double *C ,int hA,int wA,int wB)
//矩阵相乘 C=A*B，其中A矩阵是hA*wA，B矩阵是wA*wB 的，则C矩阵是hA*wB;
{
	int i,j,k;
	for (i=0;i<hA;i++)
		for(j=0;j<wB;j++)
		{
			double sum=0;
			for (k=0;k<wA;k++)
			{
				double a=A[i*wA+k];
				double b=B[k*wB+j];
				sum+=a*b;
			}
			C[i*wB+j]=sum;		
		}
}


double Registrat::GetRelate(BYTE *base,int basexsize,int baseysize,BYTE *warp,int warpxsize,int warpysize,int x, int y,int &warpx, int &warpy)
{  //所传参数行列值相反
	double *result = new double[3*3];
	BYTE *basewnd = new BYTE[3*3];
	BYTE warpwnd[5][5];

	int i,j;
	//获取warp影像上对应的点
	PreGcps(x,y,warpx,warpy);

	//取base影像上的一个3*3窗口
	for (i = 0;i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			if(x == 0)
				x = 1;
			if(y == 0)
				y = 1;
			if(x == basexsize-1)
				x = basexsize-2;
			if (y == baseysize-1)
				y = baseysize-2;

			basewnd[i*3+j] = base[(y-1+i)*basexsize+(x-1+j)];
		}
	}
	//取warp图像上5*5的窗口
	for (i = 0;i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			if(warpx == 0)
				warpx = 2;
			if(warpy == 0)
				warpy = 2;
			if(warpx == warpxsize-1)
				warpx = warpxsize-3;
			if (warpy == warpysize-1)
				warpy = warpysize-3;

			warpwnd[i][j] = warp[(y-2+i)*warpxsize+(x-2+j)];
		}
	}
	//求相关系数；共有9个相关系数
	BYTE *temp = new BYTE[3*3];
	int m , n;
	for(m = 0; m < 3; m++)
	{
		for(n = 0; n < 3; n++)
		{
			for(i = 0; i < 3; i++)
			{
				for (j = 0; j < 3; j++)
				{
					temp[i*3+j] = warpwnd[i+n][j+m];
					result[n*3+m] = corrcoef(basewnd,temp);
				}
			}
		}
	}
	//求出相关系数最大的一组
	double max = -1;
	int mark= 0;
	for (i = 0; i < 9; i++)
	{
		if(max < result[i])
		{
			max = result[i];
			mark = i;
		}
	}
	warpx = warpx + mark%3-1;
	warpy = warpy + (int)(mark/3)-1;

	delete basewnd;
	delete result;
	delete temp;

	return max;
	
}

double Registrat::corrcoef(BYTE *d1, BYTE *d2)
{
	int i;
	double xy=0, x=0, y=0, xsum=0, ysum=0;
	double corrc;
	for (i=0; i<9; i++)
	{
		xsum += d1[i];
		ysum += d2[i];
	}
	xsum /= 9;
	ysum /= 9;
	for (i=0; i<9; i++)
	{
		d1[i] -= xsum;
		d2[i] -= ysum;
		x += d1[i] * d1[i];
		y += d2[i] * d2[i];
		xy += d1[i] * d2[i];
	}
	corrc = abs(xy) / (sqrt(x) * sqrt(y));
	return corrc;
}