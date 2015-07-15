#ifndef  _GETINFO
#define  _GETINFO

class GetInfo
{
public:
	GetInfo(int xsize,int ysize):m_xsize(xsize),m_ysize(ysize){}

	void Histogram(BYTE *, float*); //ֱ��ͼ��ʾ
	void Variance(BYTE *,float&);  //����
	void Average(BYTE *, float&);   //��ֵ
	void Mod(BYTE *, int &);       //���� 
	void Max(BYTE *, int &);	 //���ֵ
	void Min(BYTE *, int &);     //���ֵ
protected:
private:
	int m_xsize;
	int m_ysize;
};



#endif