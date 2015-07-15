#ifndef  _REGISTRAT
#define  _REGISTRAT

class Registrat
{
public:
	void Getcoef();	//�����Ӧ������ϵ��
	void PreGcps(int x, int y, int &x1, int &y1); //Ԥ����Ƶ�
	double GetRelate(BYTE *,int ,int ,BYTE *,int ,int ,int x, int y,int &warpx, int &warpy); //�������ڼ������ϵ��

private:
	bool ReadGCPS();  //��ȡ���Ƶ�
	//�����������㺯��
	void swap(double *a,int i,int line,int n);
	void calculate(double *p,double *q,int n);//�γ���������
	void backcalculate(double*p,double*q,int n);//�γɵ�λ����
	void unitmatrix(double *q,int n);   //�γɵ�λ���󣬾���q��һ����λ��
	//������� C=A*B������A������hA*wA��B������wA*wB �ģ���C������hA*wB;
	void matrix_multiply(double *A,double *B,double *C ,int hA,int wA,int wB);
	double corrcoef(BYTE *d1, BYTE *d2);
	
	//��Ӧ������ϵ��������ֻ�����Կ��Ƶ㣬����ϵ��Ϊ������
	double m_a1;
	double m_b1;
	double m_c1;
	double m_a2;
	double m_b2;
	double m_c2;
	//���Ƶ��ļ�·��
	CString m_filepath;
};


#endif