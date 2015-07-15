#ifndef  _GETDATA
#define  _GETDATA

class Get_data
{
public:
	Get_data();
	~Get_data();
	bool ReadFile(CString );
	void read1band(BYTE *, int, int, int );  //做显示用
	void read3band(BYTE *,int *, int , int ); //做显示用
	void readallband(BYTE *);//数据处理
// private:
	GDALDataset *poDataset; //gdal数据集
	int nBands;  //图像波段数
	int nXsize;  //图像宽
	int nYsize; //图像的高
};

#endif
