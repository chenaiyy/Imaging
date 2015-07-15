#ifndef  _SAMPLE_
#define _SAMPLE_

#define MAXORDER	 3
#define MSUCCESS     1 /* SUCCESS */
#define MNPTERR      0 /* NOT ENOUGH POINTS */
#define MUNSOLVABLE -1 /* NOT SOLVABLE */
#define MMEMERR     -2 /* NOT ENOUGH MEMORY */
#define MPARMERR    -3 /* PARAMETER ERROR */
#define MINTERR     -4 /* INTERNAL ERROR */
#define M(row,col) m->v[(((row)-1)*(m->n))+(col)-1]

struct Control_Points
{
	int  count;
	double *e1;
	double *n1;
	double *e2;
	double *n2;
	int *status;
};

struct MATRIX
{
	int     n;     /* SIZE OF THIS MATRIX (N x N) */
	double *v;
};

class CGcpGeoCorrect
{
public:
	Control_Points	m_GcpPoint;	//控制点
	int				m_roder;	//多项式次数
	double			*m_E12;	//
	double			*m_N12;	//
	double			*m_E21;	//
	double			*m_N21;	//
public:
	CGcpGeoCorrect();
	~CGcpGeoCorrect();

public:
	int  LoadGcpPoint(CString filepath);
	void computfacter(int order);	//计算多项式系数 传入多项式次数
	void Correct(CString infile,CString outfile,int sample);
	//参考影像坐标 = 系数 * 校正影像坐标  重采样
	void GetBaseCoord(double WarpX,double WarpY,double *BaseX,double *BaseY);
	//校正影像坐标 = 系数 * 参考影像坐标  确定范围
	void GetWarpCoord(double BaseX,double BaseY,double *WarpX,double *WarpY);

private:
int CRS_georef (
			double e1,  /* EASTINGS TO BE TRANSFORMED */
			double n1,  /* NORTHINGS TO BE TRANSFORMED */
			double *e,  /* EASTINGS TO BE TRANSFORMED */
			double *n,  /* NORTHINGS TO BE TRANSFORMED */
			double E[], /* EASTING COEFFICIENTS */
			double N[], /* NORTHING COEFFICIENTS */
			int order  /* ORDER OF TRANSFORMATION TO BE PERFORMED, MUST MATCH THE
					   ORDER USED TO CALCULATE THE COEFFICIENTS */
					   );
int CRS_compute_georef_equations (struct Control_Points *cp, 
							  double E12[], double N12[], 
							  double E21[], double N21[], 
							  int order);

int calccoef(struct Control_Points *,double *,double *,int);
int calcls(struct Control_Points *,struct MATRIX *,
				  double *,double *,double *,double *);
int exactdet(struct Control_Points *,struct MATRIX *,
					double *,double *,double *,double *);
int solvemat(struct MATRIX *,double *,double *,double *,double *);
double term(int,double,double);

};

#endif