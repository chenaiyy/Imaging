#include "stdafx.h"
#include "CISODATA.h"

//======================================FeatureClass类====================================//
FeatureClass::FeatureClass()
{
	m_nClassNumber = 0;
	m_nClassPixelNum = 0;
	n_fMaxSD = 0.0;
	m_nNumOfMaxSD = 0;
	m_pCenter = 0;
	m_pStandardDifference = 0;
}

void FeatureClass::CalculateParameters()
{	////以上为计算各类的中心///
	float  pTotal = 0;
	for (int j = 0; j < m_nClassPixelNum; j ++)
	{
		pTotal  += *(m_FeatureClassData[j]);
	}
	m_pCenter= pTotal / m_nClassPixelNum;///均值


	///////////////计算各类中样本到类心的平均距离：
	float fTotal = 0;
	for (int i = 0; i < m_nClassPixelNum; i ++)
	{
		fTotal = fTotal + (*m_FeatureClassData[i] - m_pCenter)*(*m_FeatureClassData[i] - m_pCenter);
	}
	fTotal = fTotal/m_nClassPixelNum;
	m_pStandardDifference = sqrtf(fTotal);
}

//==========================================CISODATA类=========================================//
CISODATA::CISODATA()
{
	m_iClasses = 0;
	m_iIniClasses = 0;
	m_iLoopNum= 0;
	m_iMaxMergeClassNum= 0;
	m_fMinPixelNum= 0;
	m_fMaxSD= 0;
	m_fMinClassDis= 0;
	m_nTotalClassNum = 0;
}

//初始化类
bool CISODATA::InitializeClasses(void)
{
	////////// 选定初始聚类中心①初始化
	int width = poDataset->GetRasterXSize();
	int hight = poDataset->GetRasterYSize();
	int Bands = poDataset->GetRasterCount();
	//初始化输入影像数据
	BYTE *InData = new BYTE[width*hight*Bands];
	poDataset->RasterIO( GF_Read, 0, 0,width,hight, InData,width,hight, GDT_Byte,Bands, NULL,0,0,0 );
	/////////////////////得到影像矩阵范围中的宽和高中的最小值//
	int ImageRange = hight;
	if (ImageRange>width)
	{
		ImageRange =width;
	}
	///////初始化中心的采样间距
	int RangeDis = ImageRange/m_iIniClasses;
	int Location = 0;
	for (int i = 0; i < m_iIniClasses; i ++)
	{
		FeatureClass *pClass = new FeatureClass;
		float pCenter ;
		float pStandardDifference = 0;
		//为一个类申请内存后 同样为其存放Center向量和StandardDifference向量申请内存，释放时也应同时释放
		pCenter = InData[Location*width + Location];

		pClass->m_nClassNumber = i;		
		pClass->m_pCenter = pCenter;
		pClass->m_pStandardDifference = pStandardDifference;
		Location = Location + RangeDis;
		m_vClasses.push_back(pClass);
		m_nTotalClassNum++;
	}
	///////////////②按最近邻规则将样本集{xi}中每个样本分到某一类中
	for (int iBands = 0;iBands<Bands;iBands++)
	{
		for (int i = 0;i<hight;i++)
		{
			for (int j = 0;j<width;j++)
			{
				int IdClass = -1;
				float MinDis = 0;
				float Dis = 0;
				for (int iClass = 0;iClass<m_iIniClasses;iClass++)
				{
					Dis = 0;
					Dis = ((float)InData[hight*width*iBands+i*width + j] - m_vClasses[iClass]->m_pCenter)*((float)InData[hight*width*iBands+i*width + j] - m_vClasses[iClass]->m_pCenter);
					if(!iClass)
					{
						MinDis = Dis;
						IdClass = 0;
					}
					if(Dis<MinDis)
					{
						MinDis = Dis;
						IdClass = iClass;
					}
				}
				m_vClasses[IdClass]->m_FeatureClassData.push_back(InData + hight*width*iBands+i*width + j);
				m_vClasses[IdClass]->m_nClassPixelNum++;
			}
		}
	}
	return TRUE;
}

bool CISODATA::FeatureClassify(void)
{
	if(!InitializeClasses())
		return false;
	//////////////⑤判断停止、分裂或合并。
	for (int i = 0; i < m_iLoopNum; i ++) ///以下为迭代主程序a、若迭代次数Ip ＝NS，则算法结束；
	{
		for (int i = 0; i < m_nTotalClassNum; i ++)
		{
			m_vClasses[i]->CalculateParameters();			//④计算分类后的参数：各类重心、类内平均距离及总体平均距离
			if (m_vClasses[i] == NULL)
				continue;
			if(m_vClasses[i]->m_nClassPixelNum < m_fMinPixelNum)
			{///③依据TN判断合并：如果类ωj中样本数nj< TN，则取消该类的中心zj，Nc=Nc-1，转至② 。
				DelFeatureClass(m_vClasses[i]);
				vector<FeatureClass*>::iterator p = m_vClasses.begin();
				m_vClasses.erase(p + i);
			}

		}
		////b、若Nc ≤c/2，则转到⑥ （将一些类分裂）；
		if (m_nTotalClassNum <= m_iClasses / 2)
		{
			for (int j = 0; j < m_nTotalClassNum; j ++)
			{
				m_vClasses[j]->CalculateParameters();
				if (m_vClasses[j]->m_pStandardDifference > m_fMaxSD)
				{
					DivFeatureClass(m_vClasses[j]);
					vector<FeatureClass*>::iterator p = m_vClasses.begin();
					m_vClasses.erase(p + j);
				}
			}	
		}
		////c、若Nc ≥2c，则转至⑦ （跳过分裂处理）；（合并处理）。
		else if (m_nTotalClassNum >=2* m_iClasses)
		{	
			UinClassDisVec();
			for (int m = 0; m < (int)m_UinClassDis.size()&& m < m_iMaxMergeClassNum; m ++)
			{
				int nClassA = m_UinClassDis[m].nNumClassA;
				int nClassB = m_UinClassDis[m].nNumClassB;
				UniFeatureClass(m_vClasses[nClassA], m_vClasses[nClassB]);
				vector<FeatureClass*>::iterator p = m_vClasses.begin();
				m_vClasses.erase(p + nClassB);
			}

		}
		////d、若c/2< Nc<2c，当迭代次数Ip是奇数时转至⑥ （分裂处理）；迭代次数Ip是偶数时转至⑦ （合并处理）。
		else
		{
			if (m_nTotalClassNum % 2 == 1)
			{
				for (int j = 0; j < m_nTotalClassNum; j ++)
				{
					m_vClasses[j]->CalculateParameters();
					if (m_vClasses[j]->m_pStandardDifference > m_fMaxSD)
					{
						DivFeatureClass(m_vClasses[j]);
						vector<FeatureClass*>::iterator p = m_vClasses.begin();
						m_vClasses.erase(p + j);
					}
				}	
			}
			else
			{
				UinClassDisVec();
				for (int m = 0; m < (int)m_UinClassDis.size()&& m < m_iMaxMergeClassNum; m ++)
				{
					int nClassA = m_UinClassDis[m].nNumClassA;
					int nClassB = m_UinClassDis[m].nNumClassB;
					UniFeatureClass(m_vClasses[nClassA], m_vClasses[nClassB]);
					vector<FeatureClass*>::iterator p = m_vClasses.begin();
					m_vClasses.erase(p + nClassB);
				}
			}
		}
	}
	return true;

}

// 分裂类
void CISODATA::DivFeatureClass(FeatureClass* pClass)
{
	/*⑥分裂操作

	计算各类内样本到类中心的标准差向量

	σj＝(σ1j, σ2j,…., σnj)T ,      j=1,2,…..,Nc

	其各分量为：

	求出每一类内标准差向量σj中的最大分量

	若有某是所有样本在该特征上的标准差，同时又满足下面两个条件之一：

	则将该类ωj分裂为两个类，原zj取消且令Nc＝Nc＋1。

	两个新类的中心zj+和zj-分别是在原zj中相应于的分量加上和减去，而起它分量不变，其中0＜k≤1。

	分裂后，Ip=Ip+1, 转至②。*/

	if (pClass == NULL)
		return;
	float k = 0.5;				//该系数应该置为成员变量
	FeatureClass * pNewClassA = new FeatureClass;
	float  pNewCenterA ;

	FeatureClass * pNewClassS = new FeatureClass;
	float  pNewCenterS ;

	pNewCenterA = (pClass->m_pCenter) + k * pClass->m_pStandardDifference;
	pNewCenterS = (pClass->m_pCenter) - k * pClass->m_pStandardDifference;


	for (int i = 0; i < pClass->m_nClassPixelNum; i ++)
	{
		float fDistanceToNew = 0;
		float fDistanceToOld = 0;
		vector<BYTE*>::iterator p = pClass->m_FeatureClassData.begin(); //迭代器的使用，前面取消聚类中心的操作中也应该使用，但还未使用
		BYTE  *pVector = pClass->m_FeatureClassData[i];

		fDistanceToNew = fDistanceToNew + (pNewCenterA - *pVector) * (pNewCenterA - *pVector);
		fDistanceToOld = fDistanceToOld + (pNewCenterS - *pVector) * (pNewCenterS - *pVector);
		if (fDistanceToNew < fDistanceToOld)
		{
			pNewClassA->m_FeatureClassData.push_back(pVector);
			pNewClassA->m_nClassPixelNum ++;
		}
		else
		{
			pNewClassS->m_FeatureClassData.push_back(pVector);
			pNewClassS->m_nClassPixelNum ++;
		}
	}
	m_nTotalClassNum ++;
	pNewClassA->m_nClassNumber = m_nTotalClassNum - 1;
	pNewClassS->m_nClassNumber = pClass->m_nClassNumber;
	pNewClassA->m_pCenter = pNewCenterA;
	pNewClassS->m_pCenter = pNewCenterS;
	pNewClassA->CalculateParameters();
	pNewClassS->CalculateParameters();
	m_vClasses.push_back(pNewClassA);
	m_vClasses.push_back(pNewClassS);
	delete pClass;
	pClass = NULL;
}

void CISODATA::UniFeatureClass(FeatureClass * pClass,FeatureClass*pClassB)
{
	/*⑦合并操作

	计算各类中心间的距离Dij，i=1,2,…,Nc-1; j=1,2,…,Nc

	依据TC判断合并。将Dij与TC比较，并将小于TC的那些Dij按递增次序排列，取前NT个。

	从最小的Dij开始，将相应的两类合并，并计算合并后的聚类中心。

	在一次迭代中，某一类最多只能合并一次。

	Nc=Nc-已并掉的类数。
	*/
	pClass->m_pCenter = (pClass->m_nClassPixelNum*pClass->m_pCenter + pClassB->m_nClassPixelNum*pClassB->m_pCenter)/(pClass->m_nClassPixelNum+pClassB->m_nClassPixelNum);
	for (int i = 0; i < pClassB->m_nClassPixelNum; i ++)
	{
		BYTE * pfV = pClassB->m_FeatureClassData[i];
		pClass->m_FeatureClassData.push_back(pfV);
		pClass->m_nClassPixelNum++;
	}
	m_nTotalClassNum --;
	pClassB = NULL;
}

// //////////需要进行合并类两类之间的最小距离排序
void CISODATA::UinClassDisVec(void)
{
	for (int i = 0; i < m_nTotalClassNum; i ++)
	{
		m_vClasses[i]->CalculateParameters();
	}
	for (int k = 0; k < m_nTotalClassNum; k ++)
	{
		for (int h = k + 1; h < m_nTotalClassNum; h ++)
		{
			float fDis = ((m_vClasses[k]->m_pCenter) - m_vClasses[h]->m_pCenter)*(m_vClasses[k]->m_pCenter - m_vClasses[h]->m_pCenter);
			fDis = sqrtf(fDis);
			if (fDis < m_fMinClassDis)
			{
				UinClassDis clsDst;
				clsDst.fDis = fDis;
				clsDst.nNumClassA = k;
				clsDst.nNumClassB = h;
				if (m_UinClassDis.size() == 0)
					m_UinClassDis.push_back(clsDst);
				else if (m_UinClassDis.size() == 1)
				{
					if (fDis < m_UinClassDis[0].fDis)
					{
						vector<UinClassDis>::iterator p = m_UinClassDis.begin();
						m_UinClassDis.insert(p, clsDst);
					}
					else
					{
						m_UinClassDis.push_back(clsDst);
					}
				}
				else
				{
					for (int i = 0; i < m_UinClassDis.size() - 1; i ++)
					{
						if (fDis >= m_UinClassDis[i].fDis && fDis <= m_UinClassDis[i + 1].fDis)
						{
							vector<UinClassDis>::iterator p = m_UinClassDis.begin();
							m_UinClassDis.insert(p + i, clsDst);
						}
						else if (fDis < m_UinClassDis[0].fDis)
						{
							vector<UinClassDis>::iterator p = m_UinClassDis.begin();
							m_UinClassDis.insert(p, clsDst);
						}
						else
							m_UinClassDis.push_back(clsDst);
					}
				}
			}
		}
	}
}

void CISODATA::DelFeatureClass(FeatureClass *  pClass)
{
	if (pClass == NULL)
		return;

	for (int j = 0; j < pClass->m_nClassPixelNum; j ++)
	{
		int IdClass = -1;
		float MinDis = 0;
		float Dis = 0;
		for (int iClass = 0;iClass<m_nTotalClassNum;iClass++)
		{
			Dis = 0;
			Dis = (*(pClass->m_FeatureClassData[j]) - (m_vClasses[iClass]->m_pCenter))*((*pClass->m_FeatureClassData[j]) - (m_vClasses[iClass]->m_pCenter));
			if(!iClass)
			{
				MinDis = Dis;
				IdClass = 0;
			}
			if(Dis<MinDis)
			{
				MinDis = Dis;
				IdClass = iClass;
			}
		}
		m_vClasses[IdClass]->m_FeatureClassData.push_back(pClass->m_FeatureClassData[j]);
		m_vClasses[IdClass]->m_nClassPixelNum++;		
	}
	m_nTotalClassNum --;
	delete pClass;
	pClass = NULL;
}

bool CISODATA::CalClassMaxMin(void)
{
	for (int i = 0;i<m_nTotalClassNum;i++)
	{
		ClassMaxMin ClassMM;
		ClassMM.ClassID = m_vClasses[i]->m_nClassNumber;
		ClassMM.Min = 255;
		ClassMM.Max = 0;
		for (int j = 0;j<m_vClasses[i]->m_nClassPixelNum;j++)
		{
			if (ClassMM.Min>(*m_vClasses[i]->m_FeatureClassData[j]))
			{
				ClassMM.Min = (*m_vClasses[i]->m_FeatureClassData[j]);
			}
			if (ClassMM.Max<(*m_vClasses[i]->m_FeatureClassData[j]))
			{
				ClassMM.Max = (*m_vClasses[i]->m_FeatureClassData[j]);
			}
		}
		m_ClassMaxMin.push_back(ClassMM);
	}
	return true;
}

//开始分类操作

void CISODATA::Classfy()
{
	GDALAllRegister();//注册驱动
	poDataset = (GDALDataset *)GDALOpen(m_infile, GA_ReadOnly );

	FeatureClassify();
	CalClassMaxMin();
	int width = poDataset->GetRasterXSize();
	int hight = poDataset->GetRasterYSize();
	int Bands = poDataset->GetRasterCount();
	int OutBands = 3;
	//初始化输入影像数据
	BYTE *InData = new BYTE[width*hight*Bands];
	poDataset->RasterIO( GF_Read, 0, 0,width,hight, InData,width,hight, GDT_Byte,Bands, NULL,0,0,0 );
	BYTE *OutData = new BYTE[width*hight*OutBands];
	int ColorValue = 255/m_nTotalClassNum;
	for (int iBands = 0;iBands<Bands;iBands++)
	{
		for (int i = 0;i<hight;i++)
		{
			for (int j = 0;j<width;j++)
			{
				for (int iClass = 0;iClass<m_nTotalClassNum;iClass++)
				{
					ClassMaxMin MyLocal = m_ClassMaxMin[m_vClasses[iClass]->m_nClassNumber];
					if (InData[hight*width*iBands+i*width + j]>=MyLocal.Min&&InData[hight*width*iBands+i*width + j]<=MyLocal.Max)
					{
						OutData[hight*width*iBands+i*width + j] = ColorValue*MyLocal.ClassID;
						OutData[hight*width*1+i*width + j] = ColorValue*(m_nTotalClassNum-1)*MyLocal.ClassID;
						OutData[hight*width*2+i*width + j] = ColorValue*m_nTotalClassNum*MyLocal.ClassID;
						break;
					}					
				}
			}
		}
	}
	////////////////////////////////将输出影像存储为一个文件//////////////////////////
	const char *pszFormat = "bmp";

	GDALDriver *poDriver;
	poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
	GDALDataset *poDstDS;
	char **papszOptions = NULL;
	poDstDS = poDriver->Create( (char*)(LPCTSTR)m_outfile,//输出路径
		width,hight, //影像数据宽高
		OutBands, //存或取的波段数
		GDT_Byte, 	//缓冲区数据类型
		papszOptions );
	if(poDstDS->RasterIO(
		GF_Write,						//IO方式
		0,0,							//影像数据起始位置x,y
		width,hight,				//影像数据宽高
		OutData,						//指向缓冲区数据指针
		width,hight,				//缓冲区数据宽高
		GDT_Byte,						//缓冲区数据类型
		OutBands,						//存或取的波段数
		NULL,							//存或取的波段列表
		0,								//缓冲区数据一个像素占用的字节数
		0,								//缓冲区一行数据所占用的字节数
		0)==CE_Failure )				//缓冲区数据存储方式 0为RRRRGGGGBBBB-bsp 1为RGBRGBRGB-bip	
	{
		AfxMessageBox("保存文件失败！");
		return;
	}
	else
		AfxMessageBox("分类完成，请编辑！");

	GDALClose(poDstDS);
	delete []OutData;
	delete []InData;
}