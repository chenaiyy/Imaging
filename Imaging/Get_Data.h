#ifndef  _GETDATA
#define  _GETDATA

class Get_data
{
public:
	Get_data();
	~Get_data();
	bool ReadFile(CString );
	void read1band(BYTE *, int, int, int );  //����ʾ��
	void read3band(BYTE *,int *, int , int ); //����ʾ��
	void readallband(BYTE *);//���ݴ���
// private:
	GDALDataset *poDataset; //gdal���ݼ�
	int nBands;  //ͼ�񲨶���
	int nXsize;  //ͼ���
	int nYsize; //ͼ��ĸ�
};

#endif
