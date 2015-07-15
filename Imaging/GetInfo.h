#ifndef  _GETINFO
#define  _GETINFO

class GetInfo
{
public:
	GetInfo(int xsize,int ysize):m_xsize(xsize),m_ysize(ysize){}

	void Histogram(BYTE *, float*); //直方图显示
	void Variance(BYTE *,float&);  //方差
	void Average(BYTE *, float&);   //均值
	void Mod(BYTE *, int &);       //众数 
	void Max(BYTE *, int &);	 //最大值
	void Min(BYTE *, int &);     //最大值
protected:
private:
	int m_xsize;
	int m_ysize;
};



#endif