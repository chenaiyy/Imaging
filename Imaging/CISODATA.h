#ifndef  _ISODATA_
#define  _ISODATA_

#include "afxwin.h"
#include <vector>
using namespace std;

class FeatureClass
{
public:
	FeatureClass();
	virtual ~FeatureClass(){};
	float *GetClassCenter();
	inline float GetAverageDistance(){return m_fDistance;}
	void CalculateParameters();
	friend class ISODATA;
private:

public:
	int m_nClassNumber;//����
	int m_nClassPixelNum;//ÿһ�������Ԫ����
	float m_fDistance;
	float n_fMaxSD;
	float m_pCenter;
	float m_pStandardDifference;
	int m_nNumOfMaxSD;

	vector<BYTE*> m_FeatureClassData;
};

struct UinClassDis
{
	float fDis;
	int nNumClassA;
	int nNumClassB;
};

struct ClassMaxMin
{
	int ClassID;
	BYTE Max;
	BYTE Min;
};

//===================�ãɣӣϣģ��ԣ���============================================//

class CISODATA
{
public://����
	GDALDataset *poDataset;//���ݼ�����ָ��
	vector<FeatureClass*> m_vClasses;
	vector<UinClassDis> m_UinClassDis;
	vector<ClassMaxMin> m_ClassMaxMin;
	int m_iClasses;
	int m_iIniClasses;
	int m_iLoopNum;
	int m_iMaxMergeClassNum;
	float m_fMinPixelNum;
	float m_fMaxSD;
	float m_fMinClassDis;
	int m_nTotalClassNum; //ʵ�ʼ��������

	CString m_infile;   //��ȡ���ļ�
	CString m_outfile;  //�����Ӱ���ļ�

public:
	CISODATA();
	// ��ʼ����
	bool InitializeClasses(void);
	// ���з���
	bool FeatureClassify(void);
	// ������
	void DivFeatureClass(FeatureClass* pClass);
	void UniFeatureClass(FeatureClass * pClass,FeatureClass*pClassB);
	// //////////��Ҫ���кϲ�������֮�����С��������
	void UinClassDisVec(void);
	void DelFeatureClass(FeatureClass *  pClass);
	
	bool CalClassMaxMin(void);

	void Classfy();
};











#endif