#ifndef  _REGISTRAT
#define  _REGISTRAT

class Registrat
{
public:
	void Getcoef();	//计算对应函数的系数
	void PreGcps(int x, int y, int &x1, int &y1); //预测控制点
	double GetRelate(BYTE *,int ,int ,BYTE *,int ,int ,int x, int y,int &warpx, int &warpy); //滑动窗口计算相关系数

private:
	bool ReadGCPS();  //读取控制点
	//矩阵求逆运算函数
	void swap(double *a,int i,int line,int n);
	void calculate(double *p,double *q,int n);//形成上三角阵
	void backcalculate(double*p,double*q,int n);//形成单位矩阵
	void unitmatrix(double *q,int n);   //形成单位矩阵，矩阵q是一个单位阵
	//矩阵相乘 C=A*B，其中A矩阵是hA*wA，B矩阵是wA*wB 的，则C矩阵是hA*wB;
	void matrix_multiply(double *A,double *B,double *C ,int hA,int wA,int wB);
	double corrcoef(BYTE *d1, BYTE *d2);
	
	//对应函数的系数（由于只有三对控制点，所以系数为六个）
	double m_a1;
	double m_b1;
	double m_c1;
	double m_a2;
	double m_b2;
	double m_c2;
	//控制点文件路径
	CString m_filepath;
};


#endif