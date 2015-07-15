#include "stdafx.h"
#include "CISODATA.h"

//======================================FeatureClass��====================================//
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
{	////����Ϊ������������///
	float  pTotal = 0;
	for (int j = 0; j < m_nClassPixelNum; j ++)
	{
		pTotal  += *(m_FeatureClassData[j]);
	}
	m_pCenter= pTotal / m_nClassPixelNum;///��ֵ


	///////////////������������������ĵ�ƽ�����룺
	float fTotal = 0;
	for (int i = 0; i < m_nClassPixelNum; i ++)
	{
		fTotal = fTotal + (*m_FeatureClassData[i] - m_pCenter)*(*m_FeatureClassData[i] - m_pCenter);
	}
	fTotal = fTotal/m_nClassPixelNum;
	m_pStandardDifference = sqrtf(fTotal);
}

//==========================================CISODATA��=========================================//
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

//��ʼ����
bool CISODATA::InitializeClasses(void)
{
	////////// ѡ����ʼ�������Ģٳ�ʼ��
	int width = poDataset->GetRasterXSize();
	int hight = poDataset->GetRasterYSize();
	int Bands = poDataset->GetRasterCount();
	//��ʼ������Ӱ������
	BYTE *InData = new BYTE[width*hight*Bands];
	poDataset->RasterIO( GF_Read, 0, 0,width,hight, InData,width,hight, GDT_Byte,Bands, NULL,0,0,0 );
	/////////////////////�õ�Ӱ�����Χ�еĿ�͸��е���Сֵ//
	int ImageRange = hight;
	if (ImageRange>width)
	{
		ImageRange =width;
	}
	///////��ʼ�����ĵĲ������
	int RangeDis = ImageRange/m_iIniClasses;
	int Location = 0;
	for (int i = 0; i < m_iIniClasses; i ++)
	{
		FeatureClass *pClass = new FeatureClass;
		float pCenter ;
		float pStandardDifference = 0;
		//Ϊһ���������ڴ�� ͬ��Ϊ����Center������StandardDifference���������ڴ棬�ͷ�ʱҲӦͬʱ�ͷ�
		pCenter = InData[Location*width + Location];

		pClass->m_nClassNumber = i;		
		pClass->m_pCenter = pCenter;
		pClass->m_pStandardDifference = pStandardDifference;
		Location = Location + RangeDis;
		m_vClasses.push_back(pClass);
		m_nTotalClassNum++;
	}
	///////////////�ڰ�����ڹ���������{xi}��ÿ�������ֵ�ĳһ����
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
	//////////////���ж�ֹͣ�����ѻ�ϲ���
	for (int i = 0; i < m_iLoopNum; i ++) ///����Ϊ����������a������������Ip ��NS�����㷨������
	{
		for (int i = 0; i < m_nTotalClassNum; i ++)
		{
			m_vClasses[i]->CalculateParameters();			//�ܼ�������Ĳ������������ġ�����ƽ�����뼰����ƽ������
			if (m_vClasses[i] == NULL)
				continue;
			if(m_vClasses[i]->m_nClassPixelNum < m_fMinPixelNum)
			{///������TN�жϺϲ���������j��������nj< TN����ȡ�����������zj��Nc=Nc-1��ת���� ��
				DelFeatureClass(m_vClasses[i]);
				vector<FeatureClass*>::iterator p = m_vClasses.begin();
				m_vClasses.erase(p + i);
			}

		}
		////b����Nc ��c/2����ת���� ����һЩ����ѣ���
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
		////c����Nc ��2c����ת���� ���������Ѵ��������ϲ�������
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
		////d����c/2< Nc<2c������������Ip������ʱת���� �����Ѵ�������������Ip��ż��ʱת���� ���ϲ�������
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

// ������
void CISODATA::DivFeatureClass(FeatureClass* pClass)
{
	/*�޷��Ѳ���

	��������������������ĵı�׼������

	��j��(��1j, ��2j,��., ��nj)T ,      j=1,2,��..,Nc

	�������Ϊ��

	���ÿһ���ڱ�׼��������j�е�������

	����ĳ�����������ڸ������ϵı�׼�ͬʱ������������������֮һ��

	�򽫸����j����Ϊ�����࣬ԭzjȡ������Nc��Nc��1��

	�������������zj+��zj-�ֱ�����ԭzj����Ӧ�ڵķ������Ϻͼ�ȥ���������������䣬����0��k��1��

	���Ѻ�Ip=Ip+1, ת���ڡ�*/

	if (pClass == NULL)
		return;
	float k = 0.5;				//��ϵ��Ӧ����Ϊ��Ա����
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
		vector<BYTE*>::iterator p = pClass->m_FeatureClassData.begin(); //��������ʹ�ã�ǰ��ȡ���������ĵĲ�����ҲӦ��ʹ�ã�����δʹ��
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
	/*�ߺϲ�����

	����������ļ�ľ���Dij��i=1,2,��,Nc-1; j=1,2,��,Nc

	����TC�жϺϲ�����Dij��TC�Ƚϣ�����С��TC����ЩDij�������������У�ȡǰNT����

	����С��Dij��ʼ������Ӧ������ϲ���������ϲ���ľ������ġ�

	��һ�ε����У�ĳһ�����ֻ�ܺϲ�һ�Ρ�

	Nc=Nc-�Ѳ�����������
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

// //////////��Ҫ���кϲ�������֮�����С��������
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

//��ʼ�������

void CISODATA::Classfy()
{
	GDALAllRegister();//ע������
	poDataset = (GDALDataset *)GDALOpen(m_infile, GA_ReadOnly );

	FeatureClassify();
	CalClassMaxMin();
	int width = poDataset->GetRasterXSize();
	int hight = poDataset->GetRasterYSize();
	int Bands = poDataset->GetRasterCount();
	int OutBands = 3;
	//��ʼ������Ӱ������
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
	////////////////////////////////�����Ӱ��洢Ϊһ���ļ�//////////////////////////
	const char *pszFormat = "bmp";

	GDALDriver *poDriver;
	poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
	GDALDataset *poDstDS;
	char **papszOptions = NULL;
	poDstDS = poDriver->Create( (char*)(LPCTSTR)m_outfile,//���·��
		width,hight, //Ӱ�����ݿ��
		OutBands, //���ȡ�Ĳ�����
		GDT_Byte, 	//��������������
		papszOptions );
	if(poDstDS->RasterIO(
		GF_Write,						//IO��ʽ
		0,0,							//Ӱ��������ʼλ��x,y
		width,hight,				//Ӱ�����ݿ��
		OutData,						//ָ�򻺳�������ָ��
		width,hight,				//���������ݿ��
		GDT_Byte,						//��������������
		OutBands,						//���ȡ�Ĳ�����
		NULL,							//���ȡ�Ĳ����б�
		0,								//����������һ������ռ�õ��ֽ���
		0,								//������һ��������ռ�õ��ֽ���
		0)==CE_Failure )				//���������ݴ洢��ʽ 0ΪRRRRGGGGBBBB-bsp 1ΪRGBRGBRGB-bip	
	{
		AfxMessageBox("�����ļ�ʧ�ܣ�");
		return;
	}
	else
		AfxMessageBox("������ɣ���༭��");

	GDALClose(poDstDS);
	delete []OutData;
	delete []InData;
}