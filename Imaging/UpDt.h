#ifndef _UPDATA_
#define  _UPDATA_

class UpDt
{
public:
	UpDt();
	void ReadFile(CString , CString );  //读取数据
	void Sub(CString );  //进行判读
//	void OutPut(CString ); //输出影像
private:
	BYTE *m_first;
	BYTE *m_second;
	int m_nxsize;
	int m_nysize;
	int m_iclass; // 类别数
};

#endif