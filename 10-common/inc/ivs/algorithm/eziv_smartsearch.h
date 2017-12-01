#ifndef _EZIV_SMART_SEARCH_H_
#define _EZIV_SMART_SEARCH_H_

#include "ivs/eziv_commondef.h"
#include "ivs/eziv_opencvdef.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#include <iostream>

using namespace std;

#ifdef _EiC 
#define WIN32
#endif

typedef struct tagSmartSearchTemp
{
	CvRect	m_tRect;
	CvPoint m_tPoint;
	BOOL32	m_bUpdated;
}TSMARTSEARCHTEMP;

class CIntelliSearchObj
{
public:
	CIntelliSearchObj();

private:
	//���������Ѷ
	int		m_nInFrameNum;						//��������Frame��
	int		m_nInFrameCount;					//��¼������Frame Number

	//�뿪�����Ѷ	
	int		m_nOutFrameNum;						//�����뿪Frame��
	int		m_nOutFrameCount;					//��¼�뿪���Frame Number
	//���ĵ�λ��
	CvPoint m_tCenterPt;						//��¼���ĵ�λ��
	//�ж��ź�
	BOOL32	m_bPeopleIn;						//��Ա�����ź�
	BOOL32	m_bPeopleOut;						//��Ա�뿪�ź�
	BOOL32	m_bFaceDetect;						//�Ƿ���⵽��
	BOOL32	m_bDataUpdate;						//�����Ƿ��и���
	BOOL32	m_bOutputData;						//�Ƿ��������

	EM_EZIV_INTELLISEARCH_TYPE m_emIntelliType;

public:
	//////////////////////////////////////////////////////////////////////////
	//��������������ź�
	//����Ѽ�¼��Ѷ�������ã�
	void	PrintData		(int index);
	void	CountingIn		(int FrameCount);
	void	CountingOut		(int FrameCount);
	void	UpdatePosition	(CvRect PeopRect);						//�����Ѽ�¼ǰ����Ѷ
	void	ResetData		();										//������������

	void	FaceDetected	(){m_bFaceDetect = TRUE;};				//�趨��⵽�����ź�
	void	ResetUpdate		(){m_bDataUpdate = FALSE;};				//����Update�ź�

	//////////////////////////////////////////////////////////////////////////
	//�ش�����������ź�
	CvPoint	CenterPoint(){return m_tCenterPt;};						//�ش����ĵ���Ѷ
	BOOL32	Update(){return m_bDataUpdate;};						//�ش�Update�ź�
	BOOL32	In(){return m_bPeopleIn;};								//�ش�In�ź�
	BOOL32	Out(){return m_bPeopleOut;};							//�ش�Out�ź�
	BOOL32	DataOut(){return m_bOutputData;};						//�ش��Ƿ�����ź�, �ش���Ա�Ƿ����ȡData, ��Ա�Ƿ��뿪
	int		ReturnInFrameNum(){return m_nInFrameCount;};			//�ش�����ʱ��FrameNum,��Ա����ʱ��frame number
	BOOL32	ReturnFace(){return m_bFaceDetect;};					//�ش��������Ƿ�����⵽����
	int		ReturnOutFrameNum(){return m_nOutFrameCount;};			//�ش��뿪ʱ��FrameNum
};

class EASYIV_API CIntelliSearch
{
public:
	CIntelliSearch();
	~CIntelliSearch();

public:
	BOOL32		Init(TEZIVINFO_INTELLISEARCH &tParam);								//Initialize configurations
	void		DetectAndDraw(IplImage *img );										//Find faces and draw them in the image
	BOOL32		Alarm();															//Trigger when detected people left the image
	IplImage*	GetImage(EM_EZIV_IPLIMGTYPE emIplImageType);						//Get image array

public:
	void		Detect(IplImage *img2);												//Background subtraction
	void		UpdateBackgroundImage(int IntervalFrame, int select, double alpha);	//Update background image
	void		GetForeground( int background_thresh,IplImage *img);				//Get foreground image
	void		CoutTempInfo(int index);											//Output temp info
	int			ReturnObjectNum();
	void		ReturnData(int *InNum,int *OutNum,BOOL32 *face);
	void		FindMinDistance(int temp , CvPoint TempPoint,int people,CvPoint PeoplePoint);	//Find object in min distance

#ifdef _ENABLE_ENHANCEMENT_
	//���ߵ�������
	double		GAMA_TRAN(IplImage* frame,double standrand,double gama,int shift);
	//void		SetAlignmentMode(int modenumber,double newparemeter1,double newparemeter2);
	void		SetAlignmentMode(EM_EZIV_IMAGE_ENHANCE modenumber, TEZIVGamaTrans &tGamaTrans, TEZIVHistEqualize &tHistEqualize);
	void		ImageAlignment(IplImage* frame);
	void		EQUALIZE_HIST(IplImage* frame,int type,int low,int high,IplImage* img_transform);
	void		myEqulizeHist(IplImage* img,int low,int high,IplImage* img_transform);
	//���ߵ�������
#endif
private:
	TEZIVINFO_INTELLISEARCH	m_tConfig;

	CIntelliSearchObj*		m_pcPeopleInfo;			//������Ա��Ѷ	
	TSMARTSEARCHTEMP*		m_ptTempInfo;			//�ݴ滭���������Ѷ	
	CvMemStorage*			m_pMemStorage;			//����������ռ�
	CvHaarClassifierCascade*m_pCascade;				//ѵ���õķ�����

	IplImage*		m_ptFrameBG;					//����frame
	IplImage*		m_ptFrameFG;					//ǰ��frame
	IplImage*		m_ptFrameCopy;					//�ݴ�frame
	IplImage*		m_ptFrameObj;					//ǰ��������

	BOOL32			m_bAlarm;						//��⵽����ʱ�����ź�
	int				m_nObjectsInFrame;				//Ŀǰframe���������
	BOOL32			m_bOutputData;					//��Ա�뿪�Ļ�������߼�¼
	int				m_nOutputObjectNum;				//��������Ŀ
	BOOL32			m_bFaceOverlap;					//�����Ƿ��ص�
	int				m_bUpdateCount;					//��������frame
	int				m_nFrameCount;					//����frame

	//�������
	int				m_nMinDistance;					//��С����
	int				m_nMinIndex;					//��С�����������

	int				m_nInitROI_Y;					//ROI��ʼ��Y����
	int				m_nROIMaxCoordY,m_nROIMaxCoordX;		//frame�Ŀ����

	double			m_dFaceArea;					//������������ǰ�������С
	CvScalar		m_tScalarLoDiff,m_tScalarUpDiff;//
	CvPoint			m_tSeedPoint;					//��ɫ��ʼ������
	CvConnectedComp m_tConnectedComp;				//ǰ������������Ѷ
	CvRect			m_tPeopleRect;					//��������

	//���ߵ�������
	//double search_range;
#ifdef _ENABLE_ENHANCEMENT_
	double			m_dPreGama;
	double*			m_pdGamaColor;
#endif
	IplImage*		m_ptNewBG;						//����frame 
	IplImage*		m_ptHSVImageFG;
	IplImage*		m_ptHSVImageBG;					//ǰ��Ӱ��
	//���ߵ�������
	//int detect,detect2;							//����ź�
	//int initial_y,initial_size;					//face image��С��λ��
	//int initial_y2,initial_size2;					//face image��С��λ��2
	//int last_size;								//face image���մ�С
	//int length;									//ͬһ��������face image���ƶ���λ����仯�Ĵ�С
};

#endif //_EZIV_SMART_SEARCH_H_
