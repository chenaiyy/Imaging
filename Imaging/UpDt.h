#ifndef _UPDATA_
#define  _UPDATA_

class UpDt
{
public:
	UpDt();
	void ReadFile(CString , CString );  //��ȡ����
	void Sub(CString );  //�����ж�
//	void OutPut(CString ); //���Ӱ��
private:
	BYTE *m_first;
	BYTE *m_second;
	int m_nxsize;
	int m_nysize;
	int m_iclass; // �����
};

#endif