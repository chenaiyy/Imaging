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
	int m_nClassNumber;//类编号
	int m_nClassPixelNum;//每一个类的像元个数
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

//===================ＣＩＳＯＤＡＴＡ类============================================//

class CISODATA
{
public://变量
	GDALDataset *poDataset;//数据集对象指针
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
	int m_nTotalClassNum; //实际计算的类数

	CString m_infile;   //读取的文件
	CString m_outfile;  //输出的影像文件

public:
	CISODATA();
	// 初始化类
	bool InitializeClasses(void);
	// 进行分类
	bool FeatureClassify(void);
	// 分裂类
	void DivFeatureClass(FeatureClass* pClass);
	void UniFeatureClass(FeatureClass * pClass,FeatureClass*pClassB);
	// //////////需要进行合并类两类之间的最小距离排序
	void UinClassDisVec(void);
	void DelFeatureClass(FeatureClass *  pClass);
	
	bool CalClassMaxMin(void);

	void Classfy();
};











#endif